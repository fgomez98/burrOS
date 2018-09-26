#include "scheduler.h"
#include "mutex.h"
#include <pipe.h>
#include "queueADT.h"

char buff[8];
Colour colour = {255, 255, 255};
Colour yellow = {100, 1000, 255};
Colour green = {50, 50, 280};
Colour red = {100, 100, 255};

//static tProcessNode * running;
//static int amount = 0;

static queueADT ready; // colas de procesos en espera para ser ejecutados
static queueADT blocked; // cola de procesos bloqueados
static tProcess * running; // puntero al proceso que se esta corriendo en este momento

int getRunningPid() {
    return (running == NULL ) ? -1 : running->pid;
}

tProcess* getRunningProcess(){
  return running;
}


void addProcess(tProcess * p) {
    push(ready, p);
}

void blockProcess(int pid) {
    //_cli();

//    putStr("Block: ", green);
//    uintToBase(pid, buff, 10);
//    putStr(buff, yellow);
//    putChar('\n', yellow);

    changeProcessState(pid, WAITING);
    _hlt();
}

void unblockProcess(int pid) {
    //_cli();
//
//    putStr("Volvi: ", green);
//    uintToBase(pid, buff, 10);
//    putStr(buff, yellow);
//    putChar('\n', yellow);

    tProcess * elem = mallocMemory(sizeof(tProcess));
    elem->pid = pid;
    tProcess * aux = removeElem(blocked, elem);
    if (aux == NULL) {
        //_sti();
       return;
    }
    freeMemory(elem);
    aux->state = READY;
    push(ready, aux);
    //_sti();
}

void changeProcessState(int pid, pState state) {
    if (running->pid == pid) {
        running->state = state;
        return;
    }
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

tProcess * getProcessState(int pid) {
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

//        putChar('\n', yellow);
//        putStr("RunningDeadPid:", yellow);
//        uintToBase(getRunningPid(), buff, 10);
//        putStr(buff, yellow);
//        putChar('\n', yellow);

        deleteProcess(running);
    } else if (running->state == RUNNING) {
        running->state = READY;
        push(ready, running);
    } else if (running->state == WAITING) {
        push(blocked, running);
    } else if (running->state == READY) {
        push(ready, running);
    }
    while ((running = pop(ready))->state != READY) {
        if (running->state == DEAD) {

//            putChar('\n', yellow);
//            putStr("RunningDeadPid:", yellow);
//            uintToBase(getRunningPid(), buff, 10);
//            putStr(buff, yellow);
//            putChar('\n', yellow);

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

void init_(void * startingPoint) {
    ready = newQueue(sizeof(tProcess), cmpProcess);
    blocked = newQueue(sizeof(tProcess), cmpProcess);
    running = createProcess("theGodFather", startingPoint, 0, 0, NULL);
    running->state = RUNNING;
    contextSwitch(running->stackPointer);
}

void sprintProcesses(char* buffer, int buffSize){
    TNode* aux;
    int index = 0;
    int occ;
    char pid[0];
    char* mem = mallocMemory(8);
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


           occ = strcpy2(buffer+index,"   ",buffSize);
           index += occ;
           buffSize -= occ;

           intToString(mem, p->memoryAllocated);
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


             occ = strcpy2(buffer+index,running->name,buffSize);
             index += occ;
             buffSize -= occ;

             occ = strcpy2(buffer+index,"\n",buffSize);
             index += occ;
             buffSize -= occ;

            aux = aux->next;
         }
   }
   freeMemory(mem);
 }

//.........................................TESTS.........................................

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
    unblockProcess(4);
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
    push(ready, aux);
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
    unblockProcess(3);
    putStr(" Me Bloqueo ", green);
    blockProcess(getRunningPid());
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

    push(ready, proc);
    push(ready, anotherP);
    push(ready, anotherP1);

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

    putStr("My pid: 2", yellow);

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

    putStr("llegue2", colour);
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
        putStr("My pid: 3", yellow);
        putStr("valor del mutex: ", yellow);
        uintToBase(critical, buff, 10);
        putStr(buff, yellow);
        putChar('\n', yellow);
        putChar('\n', red);
        i++;
    }

    release(myMutex2);


    putStr("llegue3", colour);
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

    push(ready, proc);
    push(ready, anotherP);
    push(ready, anotherP1);
    //push(ready, anotherP2);
    //push(ready, anotherP3);
    while (1);
    endProcess(getRunningPid());
}

void pipeTestWrite() {
   pipe_t * pipeT = pipe("test");
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

    push(ready,read2);
    push(ready, write1);
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