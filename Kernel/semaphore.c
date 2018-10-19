#include "semaphore.h"
#include "VideoDriver.h"
#include "lib.h"

Colour colour6 = {255, 255, 255};
Colour red3 = {100, 100, 255};
Colour c = {215, 100, 230};
char buff6[8];
static queueADT mySems;


int cmpSem(sem * s1, sem * s2) {
    return strcmp(s1->name, s2->name);
}

sem * initSem(char * semName) {
    sem * s = mallocMemory(sizeof(sem));
    s->queue = newQueue(sizeof(int), NULL);
    s->value = 0;
    s->name = semName;
    s->lock = getMutex(semName);
    return s;
}

sem * getSem(char * semName) {
    if (mySems == NULL) {
        mySems = newQueue(sizeof(sem), cmpSem);
    }
    sem * elem = mallocMemory(sizeof(sem));
    elem->name = semName;
    sem * aux = getElem(mySems, elem);
    if (aux == NULL) {
        aux = initSem(semName);
        push(mySems, aux);
    }
    return aux;
}

void destroySem(sem * s) {
    sem * elem = mallocMemory(sizeof(sem));
    elem->name = s->name;
    sem * aux = removeElem(mySems, elem);
    if (aux != NULL) {
        freeQueueNodes(aux->queue);
        freeMemory(aux);
    }
}

void destroyAllSem() {
    sem * current;
    while ((current = pop(mySems)) != NULL) {
        freeMemory(current);
    }
}

void wait(sem * s) {
    adquire(s->lock);
    (s->value)--;
    if (s->value < 0) {
        int pid = getRunningPid();
        push(s->queue, pid);
        
//        putChar('\n', c);
//        putStr("wait: ", c);
//        uintToBase(pid, buff6, 10);
//        putStr(buff6, c);
//        putChar('\n', c);
        
        release(s->lock);
        blockProcess(pid);
    } else {
        release(s->lock);
    }
}

void post(sem * s) {
    adquire(s->lock);
    (s->value)++;
//
//    putChar('\n', c);
//    putStr("post: ", c);
//    uintToBase(getRunningPid(), buff6, 10);
//    putStr(buff6, c);
//    putChar('\n', c);

    if (s->value <= 0) {
        int pid = pop(s->queue);
        if (pid != NULL) {
            while(0 == unblockProcess(pid)) {
                putStr("desbloque un proceso muerto", red3);
                pid = pop(s->queue);
                if (pid == NULL) {
                    break;
                }
            }
        }
    }
    release(s->lock);
}
