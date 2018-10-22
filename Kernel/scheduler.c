#include "scheduler.h"
#include "mutex.h"
#include <pipe.h>
#include "queueADT.h"

char buff[8];
Colour colour = {255, 255, 255};
Colour yellow = {100, 1000, 255};
Colour green = {50, 50, 280};
Colour red = {100, 100, 255};

static queueADT priority1Queue;
static queueADT priority2Queue;
static queueADT priority3Queue;
static queueADT priority4Queue;
static queueADT priority5Queue;
static queueADT priority6Queue;
static queueADT priority7Queue;
static queueADT priority8Queue;
static queueADT priority9Queue;
static queueADT priority10Queue;
static queueADT ready; // colas de procesos en espera para ser ejecutados
static queueADT blocked; // cola de procesos bloqueados //TODO: prioridad a processos bloqueados??
static tProcess * running; // puntero al proceso que se esta corriendo en este momento
static char mode; // TODO: inicilaizar el shceuler en un modo

//TODO: si hay un proceso de prioridad mas alta ejecutamos ese, expulsando al que esta corriendo ahora?

void yield() {
    _yield();
}

int getRunningPid() {
    return (running == NULL ) ? -1 : running->pid;
}

tProcess* getRunningProcess(){
    return running;
}

void kill(int pid) {
    if (pid != 1) {
        changeProcessState(pid, DEAD);
    }
}

void runShell() {
//    if (getRunningPid() == 1) {
//        return;
//    } else {
//        
//    }
}

//TODO: nunca fue testeado
void changeToPriorityMode() {
    //_cli();
    mode = PRIORITY;
    tProcess * aux;
    while ((aux = pop(ready)) != NULL) {
        addProcess(aux);
    }
    //_sti();
}

void addAll(queueADT dst, queueADT src) {
    tProcess * aux;
    while ((aux = pop(src)) != NULL) {
        addProcess(dst);
    }
}

//TODO: nunca fue testeado
void changeToNormalMode() {
      //  _cli();
    // TODO: hay que seterar en 0 las prioridades de todos los procesos asi cuando vuelve a modo prioridad no hay problema o se re ordenan todos los procesos con la prioridades que estaban
    mode = NO_PRIORITY;
    addAll(ready, priority10Queue);
    addAll(ready, priority9Queue);
    addAll(ready, priority8Queue);
    addAll(ready, priority7Queue);
    addAll(ready, priority6Queue);
    addAll(ready, priority5Queue);
    addAll(ready, priority4Queue);
    addAll(ready, priority3Queue);
    addAll(ready, priority2Queue);
    addAll(ready, priority1Queue);
     //   _sti();
}

void addProcess(tProcess * p) {
    p->state = READY; // es necesario??
    p->quantumTime = 0;
    if (mode == NO_PRIORITY) {
        push(ready, p);
    } else {
        switch (p->priority) {
            case 1:
                push(priority1Queue, p);
                break;
            case 2:
                push(priority2Queue, p);
                break;
            case 3:
                push(priority3Queue, p);
                break;
            case 4:
                push(priority4Queue, p);
                break;
            case 5:
                push(priority5Queue, p);
                break;
            case 6:
                push(priority6Queue, p);
                break;
            case 7:
                push(priority7Queue, p);
                break;
            case 8:
                push(priority8Queue, p);
                break;
            case 9:
                push(priority9Queue, p);
                break;
            case 10:
                push(priority10Queue, p);
                break;
        }
        //        yield();
    }
}

tProcess * removeProcess(int pid) {
    if (running->pid == pid) {
        return running; // no borramos al que esta corriendo
    }
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(priority1Queue, elem);
    if (aux == NULL) {
        aux = removeElem(priority2Queue, elem);
        if (aux == NULL) {
            aux = removeElem(priority3Queue, elem);
            if (aux == NULL) {
                aux = removeElem(priority4Queue, elem);
                if (aux == NULL) {
                    aux = removeElem(priority5Queue, elem);
                    if (aux == NULL) {
                        aux = removeElem(priority6Queue, elem);
                        if (aux == NULL) {
                            aux = removeElem(priority7Queue, elem);
                            if (aux == NULL) {
                                aux = removeElem(priority8Queue, elem);
                                if (aux == NULL) {
                                    aux = removeElem(priority9Queue, elem);
                                    if (aux == NULL) {
                                        aux = removeElem(priority10Queue, elem);
                                        if (aux == NULL) {
                                            aux = removeElem(blocked, elem);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    freeMemory(elem);
    return aux;
}

void nice(int pid, int priority) {
    //_cli();
    if (mode == PRIORITY) {
        tProcess * proc = removeProcess(pid); // lo quitamos de la cola para luego insertarlo de nuevo y mantener el orden de la cola
        if (proc != NULL) {
            proc->priority = priority;
            switch (proc->state) {
                case READY:
                    addProcess(proc);
                    break;
                case WAITING:
                    push(blocked, proc); // podriamos solamente retornar el puntero para este caso pero quedaria in consistente el nombre de la funcion remove TODO: cmabiarle el nombre a la funcion remove o ver que hacer con eso
                    break;
                case RUNNING:
                    break;
                case DEAD:
                    // un proceso que esta corriendo no puede hacer nice si esta muerto, una vez sacado de su respectiva cola directamete lo borro a la mierda
                    deleteProcess(proc);
                    break;
            }
        }
    }
    //_sti();
    //TODO: si queremos desalojar al proceso que esta corriendo ahora para correr uno de prioridad superior entoces
    // yield();
}

void ageRunningProcess() {// TODO: ver que onda con el proceso padre aka shell
    if (running->priority > 1) {
        (running->priority)--;
    }
}

void pickReadyQueue() {
    if (mode == PRIORITY) {
        if (getSize(priority10Queue) > 0) {
            ready = priority10Queue;
        } else if (getSize(priority9Queue) > 0) {
            ready = priority9Queue;
        } else if (getSize(priority8Queue) > 0) {
            ready = priority8Queue;
        } else if (getSize(priority7Queue) > 0) {
            ready = priority7Queue;
        } else if (getSize(priority6Queue) > 0) {
            ready = priority6Queue;
        } else if (getSize(priority5Queue) > 0) {
            ready = priority5Queue;
        } else if (getSize(priority4Queue) > 0) {
            ready = priority4Queue;
        } else if (getSize(priority3Queue) > 0) {
            ready = priority3Queue;
        } else if (getSize(priority2Queue) > 0) {
            ready = priority2Queue;
        } else if (getSize(priority1Queue) > 0) {
            ready = priority1Queue;
        }
    }
}

void blockProcess(int pid) { //TODO: solamente se bloquean los procesos que estan correindo, quitar parametro pid
      //_cli();
//        putStr("Block: ", green);
//        uintToBase(pid, buff, 10);
//        putStr(buff, yellow);
//        putChar('\n', yellow);
   // running->state = WAITING;
    changeProcessState(pid, WAITING);
//     _sti();
    //yield();
}

int unblockProcess(int pid) {
       //_cli();
//        putStr("Volvi: ", green);
//        uintToBase(pid, buff, 10);
//        putStr(buff, yellow);
//        putChar('\n', yellow);
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(blocked, elem);
    freeMemory(elem);
    if (aux == NULL) {
                //_sti();
        return -1;
    }
    if (aux->state == DEAD) {
        deleteProcess(aux);
                //_sti();
        return 0;
    }
    //    aux->state = READY;
    //    push(ready, aux);
    addProcess(aux);
     // _sti();
    yield();
    return 1;
}

void changeProcessState(int pid, pState state) {
    //_cli();
    if (running->pid == pid) {
        running->state = state;
        //_sti();
        yield();
        //TODO: hago yield()?? o que la haga la funcion que llama a esto
        return;
    }
    putStr("LUGAR MAGICO", green); //TODO: esta al re pedo
    tProcess * aux = removeProcess(pid);
    if (aux == NULL) {
        return;
    }
    sort(aux, state);
    //_sti();
}

void sort(tProcess * p, pState state) {
    if (p->pid == 1 || state == RUNNING) {
        return;
    }
    switch (state) {
        case WAITING:
            p->state = WAITING;
            push(blocked, p);
            break;
        case DEAD:
            p->state = DEAD;
            // y que dsp el scheduler se encargue de borrarlo sino running apunta a cualquier cosa y cagamo
            // si lo saco de la colas es que no esta corriendo puedo borrarlo directo
            deleteProcess(p);
            putStr("CASO DEAD EN SORT", green);
            //push(ready, p);
            break;
        case READY:
            addProcess(p);
            //p->state = READY;
            //push(ready, p);
            break;
    }
}

tProcess * getProcessState(int pid) { //TODO: esta mal el nombre de la funcion, retoran el proceso no el estado. Quien la usa??
    if (running->pid == pid) {
        return running;
    }
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = getElem(ready, elem);
    if (aux == NULL) {
        aux = getElem(blocked, elem);
    }
    freeMemory(elem);
    return aux;
}

void scheduler() {
    if (running->state == DEAD) {
//                putStr("Dead ", red);
//                uintToBase(getRunningPid(), buff, 10);
//                putStr(buff, red);
//                putChar('\n', red);
        deleteProcess(running);
    } else if (running->state == RUNNING) {
        addProcess(running);
    } else if (running->state == WAITING) {
        push(blocked, running);
    } else if (running->state == READY) {
        putStr("SCHEDULER ENCONTRO UN PROCESSO EN READY", green); //TODO: esta al re pedo
        //push(ready, running);
        addProcess(running);
    }
    pickReadyQueue();
    while ((running = pop(ready))->state != READY) {
                if (running->state == DEAD) {
                    putStr("SCHEDULER ENCONTRO UN PROCESSO EN DEAD", green); //TODO: esta al re pedo
                    deleteProcess(running);
                } else if (running->state == WAITING) {
                    putStr("SCHEDULER ENCONTRO UN PROCESSO EN WAITING", green); //TODO: esta al re pedo
                    push(blocked, running);
                }
        pickReadyQueue();
    }
    running->state = RUNNING;
}

void * dispatcher(int rsp) {
    if (running == NULL) {
        return rsp;
    }
    running->stackPointer = rsp;
    running->quantumTime++;
    if (mode == PRIORITY && running->quantumTime >= running->priority) {
        ageRunningProcess();
        scheduler();
    } else {
        scheduler();
    }
    return running->stackPointer;
}

/**
 Funcion que compara dos rocesos p1, p2 segun sus pids retorna (> 0) si p1 > p2 (< 0) caso contrario 0 si son iguales
 */
int cmpProcess(tProcess * p1, tProcess * p2) {
    return (p1->pid) - (p2->pid);
}

int cmpPriority(tProcess * p1, tProcess * p2) {
    return (p1->priority) - (p2->priority);
}


void init_(void * startingPoint) {
    // STATE PIORITY MODE
    mode = PRIORITY;
    
    // INITIALIZE QUEUES
    ready; // = newQueue(sizeof(tProcess), cmpProcess);
    priority1Queue = newQueue(sizeof(tProcess), cmpProcess);
    priority2Queue = newQueue(sizeof(tProcess), cmpProcess);
    priority3Queue=  newQueue(sizeof(tProcess), cmpProcess);
    priority4Queue=  newQueue(sizeof(tProcess), cmpProcess);
    priority5Queue=  newQueue(sizeof(tProcess), cmpProcess);
    priority6Queue = newQueue(sizeof(tProcess), cmpProcess);
    priority7Queue = newQueue(sizeof(tProcess), cmpProcess);
    priority8Queue=  newQueue(sizeof(tProcess), cmpProcess);
    priority9Queue=  newQueue(sizeof(tProcess), cmpProcess);
    priority10Queue=  newQueue(sizeof(tProcess), cmpProcess);
    blocked = newQueue(sizeof(tProcess), cmpProcess);
    
    //RUN FIRST PROCESS
    running = createProcess("theGodFather", startingPoint, 0, 0, NULL);
    running->state = RUNNING;
    contextSwitch(running->stackPointer);
}

void sprintProcesses(char* buffer, int buffSize){
    TNode* aux;
    int index = 0;
    int occ;
    char pid[0];
    char* mem[8];
    //char* mem = mallocMemory(8);
    int s;
    char* states[4];
    states[0] = "ready";
    states[1] = "running";
    states[2] = "waiting";
    states[3] = "dead";
    
    intToString(pid, running->pid);
    occ = strcpy2(buffer+index,pid,buffSize);
    index += occ;
    buffSize -= occ;
    occ = strcpy2(buffer+index,"     ",buffSize);
    index += occ;
    buffSize -= occ;
    
    s = stateIdentifier(running->state);
    occ = strcpy2(buffer+index,states[s],buffSize);
    index+=occ;
    buffSize-=occ;
    
    occ = strcpy2(buffer+index,"     ",buffSize);
    index += occ;
    buffSize -= occ;
    
    
    intToString(mem, running->memoryAllocated);
    occ = strcpy2(buffer+index,mem,buffSize);
    index += occ;
    buffSize -= occ;
    
    
    occ = strcpy2(buffer+index,"             ",buffSize);
    index += occ;
    buffSize -= occ;
    
    
    
    occ = strcpy2(buffer+index,running->name,buffSize);
    index += occ;
    buffSize -= occ;
    
    occ = strcpy2(buffer+index,"\n",buffSize);
    index += occ;
    buffSize -= occ;
    
    
    
    if(ready != NULL){
        aux = ready->first;
        while(aux!= NULL){
            if(buffSize<=20) break;
            
            tProcess* p = aux->elem;
            
            intToString(pid, p->pid);
            occ = strcpy2(buffer + index, pid, buffSize);
            index += occ;
            buffSize -= occ;
            
            
            occ = strcpy2(buffer+index,"     ",buffSize);
            index+=occ;
            buffSize-=occ;
            
            
            s = stateIdentifier(p->state);
            occ = strcpy2(buffer+index,states[s],buffSize);
            index+=occ;
            buffSize-=occ;
            
            
            occ = strcpy2(buffer+index,"     ",buffSize);
            index += occ;
            buffSize -= occ;
            
            intToString(mem, p->memoryAllocated);
            occ = strcpy2(buffer+index,mem,buffSize);
            index += occ;
            buffSize -= occ;
            
            occ = strcpy2(buffer+index,"             ",buffSize);
            index += occ;
            buffSize -= occ;
            
            
            occ = strcpy2(buffer+index,p->name,buffSize);
            index += occ;
            buffSize -= occ;
            
            occ = strcpy2(buffer+index,"\n",buffSize);
            index += occ;
            buffSize -= occ;
            
            
            
            
            aux = aux->next;
        }
        
    }
    
    if(blocked != NULL){
        aux = blocked->first;
        while(aux!= NULL){
            if(buffSize<=20) break;
            
            tProcess* p = aux->elem;
            
            intToString(pid, p->pid);
            occ = strcpy2(buffer + index, pid, buffSize);
            index += occ;
            buffSize -= occ;
            
            
            occ = strcpy2(buffer+index,"     ",buffSize);
            index+=occ;
            buffSize-=occ;
            
            
            s = stateIdentifier(p->state);
            occ = strcpy2(buffer+index,states[s],buffSize);
            index+=occ;
            buffSize-=occ;
            
            
            occ = strcpy2(buffer+index,"     ",buffSize);
            index += occ;
            buffSize -= occ;
            
            intToString(mem, p->memoryAllocated);
            occ = strcpy2(buffer+index,mem,buffSize);
            index += occ;
            buffSize -= occ;
            
            occ = strcpy2(buffer+index,"             ",buffSize);
            index += occ;
            buffSize -= occ;
            
            
            occ = strcpy2(buffer+index,p->name,buffSize);
            index += occ;
            buffSize -= occ;
            
            occ = strcpy2(buffer+index,"\n",buffSize);
            index += occ;
            buffSize -= occ;
            
            aux = aux->next;
        }
    }
    //freeMemory(mem);
}

//.........................................TESTS.........................................


void priority1() {
    int pid = getRunningPid();
    int priority;
    int i = 0;
    while (i < 300) {
        priority = (getRunningProcess())->priority;
        putStr("My Pid:", red);
        uintToBase(pid, buff, 10);
        putStr(buff, red);
        putStr("priority:", yellow);
        uintToBase(priority, buff, 10);
        putStr(buff, yellow);
        putChar('\n', red);
    }
}

void priority2() {
    int pid = getRunningPid();
    int priority;
    while (1) {
        priority = (getRunningProcess())->priority;
        putStr("My Pid:", red);
        uintToBase(pid, buff, 10);
        putStr(buff, red);
        putStr("priority:", yellow);
        uintToBase(priority, buff, 10);
        putStr(buff, yellow);
        putChar('\n', red);
    }
}

void priority3() {
    int pid = getRunningPid();
    int priority;
    while (1) {
        priority = (getRunningProcess())->priority;
        putStr("My Pid:", red);
        uintToBase(pid, buff, 10);
        putStr(buff, red);
        putStr("priority:", yellow);
        uintToBase(priority, buff, 10);
        putStr(buff, yellow);
        putChar('\n', red);
    }
}

void priorityTest() {
    tProcess * proc = createProcess("maite capa", priority1, 0, 0, NULL);
    printProcess(proc);
    
    tProcess * anotherP = createProcess("fer0", priority1, 0, 0, NULL);
    printProcess(anotherP);
    
    tProcess * anotherP1 = createProcess("fer1", priority1, 0, 0, NULL);
    printProcess(anotherP1);
    
    dumpMemory();
    
    //    push(ready, proc);
    //    push(ready, anotherP);
    //    push(ready, anotherP1);
    
    addProcess(proc);
    addProcess(anotherP);
    addProcess(anotherP1);
    
    int i;
    
    i = 0;
    while (i < 90000000) {
        i++;
    }
    
    
    nice(2, 5);
    nice(3, 7);
    nice(4, 10);
    
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel2() {
    int i = 0;
    while (i < 100) {
        putStr(" Cosa ", colour);
        i++;
    }
    putStr(" Mato al 4 ", green);
    endProcess(4);
//    putStr(" Me Bloqueo ", green);
//    blockProcess(getRunningPid());
    i = 0;
    while (i < 100) {
        putStr(" Cosa ", colour);
        //i++;
    }
    //unblockProcess(4);
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel5() {
    int i = 0;
    while (i < 100) {
        putStr("My pid:  ", colour);
        uintToBase(getRunningPid(), buff, 10);
        putStr(buff, colour);
        putChar('\n', colour);
        i++;
    }
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel4() {
    tProcess * aux = createProcess("Imprime", probandoEscribirEnKernel5, 0, 0, NULL);
    //push(ready, aux);
    addProcess(aux);
    int i = 0;
    while (i < 1000) {
        putStr("Hola", colour);
        i++;
    }
    endProcess(getRunningPid());
}

void probandoEscribirEnKernel3() {
    int i = 0;
    while (i < 500) {
        putStr(" Vamo ", colour);
        i++;
    }
//    unblockProcess(3);
//    putStr(" Me Bloqueo ", green);
//    blockProcess(getRunningPid());
    i = 0;
    while (i < 100) {
        putStr(" Vamo ", colour);
        i++;
    }
    
    endProcess(getRunningPid());
}
void probandoEscribirEnKernel() {
    int i = 0;
    while (i <1500) {
        putStr(" Genere ", colour);
        i++;
    }
//     putStr(" Mato al 3 ", green);
//    endProcess(3);
    dumpMemory();
    while(1) {
        putStr(" Genere ", colour);
    }
    endProcess(getRunningPid());
}

void init_Process() {
    tProcess * proc = createProcess("maite capa", probandoEscribirEnKernel, 0, 0, NULL);
    printProcess(proc);
    
    tProcess * anotherP = createProcess("fer0", probandoEscribirEnKernel2, 0, 0, NULL);
    printProcess(anotherP);
    
    tProcess * anotherP1 = createProcess("fer1", probandoEscribirEnKernel3, 0, 0, NULL);
    printProcess(anotherP1);
    
    dumpMemory();
    
    //    push(ready, proc);
    //    push(ready, anotherP);
    //    push(ready, anotherP1);
    
    addProcess(proc);
    addProcess(anotherP);
    addProcess(anotherP1);
    
    probandoEscribirEnKernel4();
    //endProcess(getRunningPid());
}

//static mutex * myMutex;
static int critical;

void mutexTest1() {
    mutex * myMutex =getMutex("myMutex");
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    //    putStr("Dir2: ", yellow);
    //    uintToBase(myMutex, buff, 10);
    //    putStr(buff, green);
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    
    adquire(myMutex);
    
    putStr("My pid: ", yellow);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    
    critical++;
    putStr("valor del mutex: ", yellow);
    uintToBase(critical, buff, 10);
    putStr(buff, yellow);
    putChar('\n', yellow);
    int i = 0;
    while (i < 500000) {
        i++;
    }
    
    release(myMutex);
    
    i = 0;
    while (i<500) {
        i++;
    }
    
    i = 0;
    
    
    adquire(myMutex);
    
    while (i < 5) {
        critical++;
        putStr("My pid: ", yellow);
        uintToBase(getRunningPid(), buff, 10);
        putStr(buff, yellow);
        putStr("valor del mutex: ", yellow);
        uintToBase(critical, buff, 10);
        putStr(buff,yellow);
        putChar('\n', yellow);
        i++;
    }
    
    release(myMutex);
    
    putStr("llegue", colour);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    endProcess(getRunningPid());
}

void mutexTest2() {
    mutex * myMutex2 =getMutex("myMutex");
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    //    putStr("Dir3: ", yellow);
    //    uintToBase(myMutex2, buff, 10);
    //    putStr(buff, green);
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    
    int i = 0;
    while (i<90000) {
        i++;
    }
    
    i = 0;
    
    
    adquire(myMutex2);
    
    putChar('\n', red);
    putChar('\n', red);
    putStr("My pid: ", yellow);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    
    critical++;
    putStr("valor del mutex: ", yellow);
    uintToBase(critical, buff, 10);
    putStr(buff,yellow);
    putChar('\n', yellow);
    putChar('\n', red);
    
    
    release(myMutex2);
    
    
    i = 0;
    while (i<500) {
        i++;
    }
    
    i = 0;
    
    adquire(myMutex2);
    
    while (i < 5) {
        critical++;
        putStr("My pid: ", yellow);
        uintToBase(getRunningPid(), buff, 10);
        putStr(buff, yellow);
        putStr("valor del mutex: ", yellow);
        uintToBase(critical, buff, 10);
        putStr(buff, yellow);
        putChar('\n', yellow);
        putChar('\n', red);
        i++;
    }
    
    release(myMutex2);
    
    
    putStr("llegue", colour);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    endProcess(getRunningPid());
}


void mutexTest3() {
    mutex * myMutex3 = getMutex("myMutex");
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    //    putStr("Dir4: ", yellow);
    //    uintToBase(myMutex3, buff, 10);
    //    putStr(buff, green);
    //    putChar('\n', yellow);
    //    putChar('\n', yellow);
    
    
    adquire(myMutex3);
    
    putStr("My pid: ", yellow);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    critical--;
    putStr("valor del mutex: ", yellow);
    uintToBase(critical, buff, 10);
    putStr(buff,yellow);
    putChar('\n', yellow);
    putChar('\n', red);
    
    release(myMutex3);
    
    int i = 0;
    while (i<500) {
        i++;
    }
    
    i = 0;
    
    adquire(myMutex3);
    while (i < 5) {
        critical--;
        putStr("My pid: ", yellow);
        uintToBase(getRunningPid(), buff, 10);
        putStr(buff, yellow);
        putStr("valor del mutex: ", yellow);
        uintToBase(critical, buff, 10);
        putStr(buff, yellow);
        putChar('\n', yellow);
        putChar('\n', red);
        i++;
    }
    
    release(myMutex3);
    putStr("llegue", colour);
    uintToBase(getRunningPid(), buff, 10);
    putStr(buff, yellow);
    putStr("valor del mutex: ", yellow);
    uintToBase(critical, buff, 10);
    putStr(buff, yellow);
    
    endProcess(getRunningPid());
    
}

void mutexTest() {
    //myMutex = initMutex("myMutex");
    critical = 1;
    tProcess * proc = createProcess("mutez1", mutexTest1, 0, 0, NULL);
    printProcess(proc);
    
    tProcess * anotherP = createProcess("mutex2", mutexTest2, 0, 0, NULL);
    printProcess(anotherP);
    
    tProcess * anotherP1 = createProcess("mutex3", mutexTest3, 0, 0, NULL);
    printProcess(anotherP1);
    
    tProcess * anotherP2 = createProcess("mutex3", mutexTest3, 0, 0, NULL);
    printProcess(anotherP2);
    
    tProcess * anotherP3 = createProcess("mutex2", mutexTest2, 0, 0, NULL);
    
    
    //dumpMemory();
    
    //    push(ready, proc);
    //    push(ready, anotherP);
    //    push(ready, anotherP1);
    //push(ready, anotherP2);
    //push(ready, anotherP3);
    addProcess(proc);
    addProcess(anotherP);
    //addProcess(anotherP1);
    //addProcess(anotherP2);
    //addProcess(anotherP3);
    while (1);
    endProcess(getRunningPid());
}

void pipeTestWrite() {
    pipe_t * pipeT = pipe("test");
    putStr("escribo",yellow);
    writePipe(pipeT,"holaholaholaholaholaholaholahola", 8);
    endProcess(getRunningPid());
}
void pipeTest1() {
    pipe_t * pipeT = pipe("test");
    char * resp = mallocMemory(5);
    
    readPipe(pipeT,resp, 4);
    resp[4] = '\0';
    putStr(resp,yellow);
    endProcess(getRunningPid());
}

void pipeTest2() {
    pipe_t * pipeT = pipe("test");
    char * resp = mallocMemory(5);
    
    readPipe(pipeT,resp, 4);
    resp[4] = '\0';
    putStr(resp,yellow);
    endProcess(getRunningPid());
}

void pipeTest3() {
    pipe_t * pipeT = pipe("test");
    char * resp = mallocMemory(5);
    
    readPipe(pipeT,resp, 4);
    resp[4] = '\0';
    putStr(resp,yellow);
    endProcess(getRunningPid());
}



void pipeTest() {
    //myMutex = initMutex("myMutex");
    critical = 1;
    tProcess * write1 = createProcess("escritura", pipeTestWrite, 0, 0, NULL);
    
    tProcess * write2 = createProcess("escritura2", pipeTestWrite, 0, 0, NULL);
    
    tProcess * read1 = createProcess("lectura2", pipeTest2, 0, 0, NULL);
    
    tProcess * read2 = createProcess("lectura3", pipeTest3, 0, 0, NULL);
    
    tProcess * read3 = createProcess("lecura3", pipeTest1, 0, 0, NULL);
    
    
    //dumpMemory();
    
    push(ready,write1);
    push(ready, read2);
    push(ready, read1);
    push(ready, read3);
    float i = 0;
    while(i<1000000)
        i+=0.1;
    push(ready, write2);
    
    // push(ready, anotherP1);
    //push(ready, anotherP2);
    while (1);
    endProcess(getRunningPid());
    
}