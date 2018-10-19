#include "mutex.h"
#include "VideoDriver.h"
#include "lib.h"
#include "String.h"
Colour colour5 = {255, 255, 255};
Colour red2 = {100, 100, 255};
char buff5[8];
static queueADT myMutexs;


int cmpMutex(mutex * m1, mutex * m2) {
    return strcmp(m1->name, m2->name);
}

/**
 Crea un mutex dado un nombre y retorna un puntero al mismo
 */
mutex * initMutex(char * mutexName) {
    mutex * m = mallocMemory(sizeof(mutex));
    m->queue = newQueue(sizeof(int), NULL);
    m->value = 0;
    m->name = mutexName;
    return m;
}

/**
 Se fija si el mutex con ese nombre existe, si existe retorna un puntero a el sino crea un mutex dado un nombre y retorna un puntero al mismo
 */
mutex * getMutex(char * mutexName) {
    if (myMutexs == NULL) {
        myMutexs = newQueue(sizeof(mutex), cmpMutex);
    }
    mutex * elem = mallocMemory(sizeof(mutex));
    elem->name = mutexName;
    mutex * aux = getElem(myMutexs, elem);
    if (aux == NULL) {
        aux = initMutex(mutexName);
        push(myMutexs, aux);
    }
    return aux;
}

/**
 Elmina el mutex y  libera sus recursos
 */
void destroyMutex(mutex * m) {
    mutex * elem = mallocMemory(sizeof(mutex));
    elem->name = m->name;
    mutex * aux = removeElem(myMutexs, elem);
    if (aux != NULL) {
        freeQueueNodes(aux->queue);
        freeMemory(aux);
    }
}

/**
 El proceso que llama toma el mutex
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este sera bloqueado
 funcion atomica
 */
void adquire(mutex * m) {
    _cli();
    if(swapLock(&(m->value))) {
        int pid = getRunningPid();
        push(m->queue, pid);
        blockProcess(pid);
         _sti();
    }
//        if (strcmp("prodConsBufferMutex", m->name) == 0) {
//            putChar('\n', red2);
//            putStr("adquire by: ", red2);
//            uintToBase(getRunningPid(), buff5, 10);
//            putStr(buff5, red2);
//            putChar('\n', red2);
//        }
    _sti();
}

/**
 El proceso que es dueño del mutex es el unico que puede liberarlo
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este no sera liberado
 funcion atomica
 */
void release(mutex * m) {
    _cli();
//         if (strcmp("prodConsBufferMutex", m->name) == 0) {
//             putChar('\n', red2);
//             putStr("released by: ", red2);
//             uintToBase(getRunningPid(), buff5, 10);
//             putStr(buff5, red2);
//             putChar('\n', red2);
//         }

        int pid = pop(m->queue);
        if (pid != NULL) {
            if (0 == unblockProcess(pid)) {
                putStr("desbloque un proceso muerto", red2);
                release(m);
            }
        } else {
            swapUnLock(&(m->value));
        }
    _sti();
    //}
}

/**
 Destruye todos los mutex liberando el espacio de memoria reservado
 */
void destroyAllMutexs() {
    mutex * current;
    while ((current = pop(myMutexs)) != NULL) {
        freeMemory(current);
    }
}

