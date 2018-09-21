#include "scheduler.h"

char buff[8];
Colour colour = {255, 255, 255};
Colour yellow = {100, 1000, 255};
Colour green = {50, 50, 280};

//static tProcessNode * running;
//static int amount = 0;

static queueADT ready; // colas de procesos en espera para ser ejecutados
static queueADT blocked; // cola de procesos bloqueados
static tProcess * running; // puntero al proceso que se esta corriendo en este momento

void probandoEscribirEnKernel2() {
    int i = 0;
    while (i < 100) {
       putStr(" Cosa ", colour);
        i++;
    }
    putStr(" Me Bloqueo ", green);
    blockProcess(getRunningPid());
    i = 0;
    while (i < 100) {
        putStr(" Cosa ", colour);
        i++;
    }
    endProcess();
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
    endProcess();
}

void probandoEscribirEnKernel4() {
        tProcess * aux = createProcess("Imprime", probandoEscribirEnKernel5, 0, 0, NULL);
        push(ready, aux);
        int i = 0;
        while (i < 1000) {
            putStr("Hola", colour);
            i++;
        }
        endProcess();
}

void probandoEscribirEnKernel3() {
    int i = 0;
    while (i < 500) {
        putStr(" Vamo ", colour);
        i++;
    }
    unblockProcess(3);
    endProcess();
}
void probandoEscribirEnKernel() {
    int i = 0;
    while (i <1500) {
        putStr(" Genere ", colour);
        i++;
    }
    dumpMemory();
    while(1) {
        putStr(" Genere ", colour);
    }
    endProcess();
}

int getRunningPid() {
    return (running == NULL ) ? -1 : running->pid;
}

void addProcess(tProcess * p) {
    push(ready, p);
}

void blockProcess(int pid) {
    _cli();
    changeProcessState(pid, WAITING);
    _hlt();
}

void unblockProcess(int pid) {
    _cli();
    putStr("Volvi", green);
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(blocked, elem);
    if (aux == NULL) {
        _sti();
       return;
    }
    freeMemory(elem);
    aux->state = READY;
    push(ready, aux);
    _sti();
}

changeProcessState(int pid, pState state) {
    if (running->pid == pid) {
        running->state = state;
        
        putChar('\n', yellow);
        putStr("RunningDead Pid:", yellow);
        uintToBase(getRunningPid(), buff, 10);
        putStr(buff, yellow);
        putChar('\n', yellow);
        
        return;
    }
    //TODO: esta parte del codigo jamas fue testada, nose si se usara en algun caso mucho sentido no tiene
    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(ready, elem);
    if (aux == NULL) {
        aux = removeElem(blocked, elem);
        freeMemory(elem);
        aux->state = state;
        push(blocked, aux);
        return;
    }
    freeMemory(elem);
    aux->state = state;
    push(ready, aux);
}

//void runNextProcess() { // yield
//    if (running != NULL) {
//        addProcess(running);
//    }
//    running = pop(ready); // que pasa si ready esta vacia??
//    contextSwitch(running->stackPointer);
//}


void runNextProcess() {
//    _hlt();
    scheduler();
    contextSwitch(running->stackPointer);
}

//void scheduler() {
//    push(ready, running);
//    running->state = READY;
//    running = pop(ready);
//    running->state = RUNNING;
//}

void scheduler() {
    if (running->state == DEAD) {
        deleteProcess(running);
    } else if (running->state == RUNNING) {
        running->state = READY;
        push(ready, running);
    } else if (running->state == WAITING) {
        push(blocked, running);
    }
    while ((running = pop(ready))->state != READY) {
        if (running->state == DEAD) {
            deleteProcess(running);
        } else if (running->state == WAITING) {
            push(blocked, running);
        }
    }
    running->state = RUNNING;
}

void * dispatcher(int rsp) {
    if (running == NULL) {
        return rsp;
    }
    running->stackPointer = rsp;
    scheduler();
    return running->stackPointer;
}

/**
 Funcion que compara dos rocesos p1, p2 segun sus pids retorna (> 0) si p1 > p2 (< 0) caso contrario 0 si son iguales
 */
int cmpProcess(tProcess * p1, tProcess * p2) {
    return (p1->pid) - (p2->pid);
}

void init_Process() {
    tProcess * proc = createProcess("maite capa", probandoEscribirEnKernel, 0, 0, NULL);
    printProcess(proc);
    
    tProcess * anotherP = createProcess("fer0", probandoEscribirEnKernel2, 0, 0, NULL);
    printProcess(anotherP);
    
    tProcess * anotherP1 = createProcess("fer1", probandoEscribirEnKernel3, 0, 0, NULL);
    printProcess(anotherP1);
    
    dumpMemory();
    
    push(ready, proc);
    push(ready, anotherP);
    push(ready, anotherP1);

    //probandoEscribirEnKernel4();
    endProcess();
}

void init_(void * startingPoint) {
    ready = newQueue(sizeof(tProcess), cmpProcess);
    blocked = newQueue(sizeof(tProcess), cmpProcess);
    running = createProcess("theGodFather", startingPoint, 0, 0, NULL);
    running->state = RUNNING;
    //printProcess(running);
    contextSwitch(running->stackPointer);
}

//void * dispatcher(int rsp) {
//    if (running != NULL) {
//        running->stackPointer = rsp;
//        push(ready, running);
//    }
//    running = pop(ready);
//    if (running != NULL) {
//        return running->stackPointer;
//    }
//    return rsp;
//}

//void * dispatcher(int rsp) {
//    if (running == NULL) {
//        running = pop(ready);
//    } else {
//        running->stackPointer = rsp;
//        running = pop(ready);
//    }
//    return running->stackPointer;
//}

//void addProcess(tProcess * process) {
//    tProcessNode * node = mallocMemory(sizeof(tProcessNode));
//    node->process = process;
//    if (running == NULL) {
//        running = node;
//        running->next = running;
//    } else {
//        node->next = running->next;
//        running->next = node;
//    }
//    amount++;
//}
//
////void removeProcess(int pid) {
////    if (running == NULL) {
////        return;
////    } else if(cmpProcess(running->process, running->next->process) && running->process->pid == pid) {
////        deleteProcess(running->process);
////        freeMemory(running);
////        running = NULL;
////        amount--;
////        return;
////    }
////    tProcessNode * prev = running;
////    tProcessNode * current = running->next;
////    while (current->process->pid != pid) {
////        prev = current;
////        current = current->next;
////    }
////    if (cmpProcess(current->process, running->process)) {
////        running = running->next;
////    }
////    prev->next = current->next;
////    amount--;
////    addToFreeQueue(slotToRemove);
////    deleteProcess(current->process);
////    freeMemory(current);
////}
//
//
//void getNextProcess() {
//    if (running->process->state = RUNNING) {
//        running->process->state = READY;
//    }
//    running = running->next;
//    while (running->process->state != READY) {
//        running = running->next;
//    }
//    running->process->state = RUNNING;
//}
//
//void * dispatcher(int rsp) {
//    if (running == NULL) {
//        return rsp;
//    }
//    running->process->stackPointer = rsp;
//    getNextProcess();
//    return running->process->stackPointer;
//}
//
