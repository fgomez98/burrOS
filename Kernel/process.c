#include "process.h"
#include "scheduler.h"
#include "syscalls.h"

char buff2[8];
Colour colour2 = {255, 255, 255};
Colour yellow2 = {100, 1000, 255};
static int nextPid = 1; //Esta variable le asigna a cada proceso un pid distinto

//tProcess* createProcess(char* processName,void* startingPoint, int parentPid, int argc, char* argv[]){
//    /*Se reserva espacio para la estructura del proceso*/
//    tProcess* process = mallocMemory(sizeof(tProcess));
//
//    /*Se completan los campos del nuevo proceso*/
//    process->pid = nextPid++;
//    process->parentPid = parentPid;
//    process->name = processName;
//    process->processMemoryLowerAddress = mallocMemory(PROCESS_SIZE);
//    
//    void* processMemoryUpperAddress = process->processMemoryLowerAddress + PROCESS_SIZE -1;
//    process->stackPointer = initializeStack(processMemoryUpperAddress - sizeof(tStackFrame) +1 , argc, argv, startingPoint);
//    process->memoryAllocated = PROCESS_SIZE;
//    process->state = READY;
//    process->heap = NULL;
//    return process;
//}
//
////RowDaBoat
//void* initializeStack(void * stackPointer, int argc, char* argv[], void * startingPoint) {
//    tStackFrame* stackFrame = (tStackFrame *) (stackPointer );
//    stackFrame->gs= 0x001;
//    stackFrame->fs= 0x002;
//    stackFrame->r15= 0x003;
//    stackFrame->r14= 0x004;
//    stackFrame->r13= 0x005;
//    stackFrame->r12= 0x006;
//    stackFrame->r11= 0x007;
//    stackFrame->r10= 0x008;
//    stackFrame->r9=	0x009;
//    stackFrame->r8= 0x00A;
//    stackFrame->rsi= (uint64_t)argv;
//    stackFrame->rdi= (uint64_t)argc;
//    stackFrame->rbp= 0x00D;
//    stackFrame->rdx= 0x00;
//    stackFrame->rcx= 0x00F;
//    stackFrame->rbx= 0x010;
//    stackFrame->rax= 0x011;
//    stackFrame->rip= (uint64_t)startingPoint;
//    stackFrame->cs=	0x008;
//    stackFrame->eflags= 0x202;
//    stackFrame->rsp= (uint64_t)&(stackFrame->base); //el rsp apunta a la direccion en la que esta el base
//    stackFrame->ss= 0x000;
//    stackFrame->base= 0x000;
//    return stackFrame;
//}

tProcess* createProcess(char* processName,void* startingPoint, int parentPid, int argc, char* argv[]){
    /*Se reserva espacio para la estructura del proceso*/
    tProcess* process = mallocMemory(sizeof(tProcess));

    /*Se completan los campos del nuevo proceso*/
    process->pid = nextPid++;
    process->parentPid = parentPid;
    process->name = processName;
    process->processMemoryLowerAddress = mallocMemory(PROCESS_SIZE);
    
    process->code = startingPoint;
    
    void* processMemoryUpperAddress = process->processMemoryLowerAddress + PROCESS_SIZE -1;
    process->stackPointer = processMemoryUpperAddress - sizeof(tStackFrame) + 1;
    initializeStack(process->stackPointer, argc, argv, startingPoint);
    process->memoryAllocated = PROCESS_SIZE;
    process->state = READY;
    process->heap = NULL;
    return process;
}

void runProcess(int argc, char * argv[], void * startingPoint) { // por orden de como se levantan los argumentos
    ((int (*)(int, void**))(startingPoint))(argc, argv);
    _killCurrentProcess();
    
}

//RowDaBoat
void initializeStack(tStackFrame * stackFrame, int argc, char* argv[], void * startingPoint) {
    stackFrame->gs= 0x001;
    stackFrame->fs= 0x002;
    stackFrame->r15= 0x003;
    stackFrame->r14= 0x004;
    stackFrame->r13= 0x005;
    stackFrame->r12= 0x006;
    stackFrame->r11= 0x007;
    stackFrame->r10= 0x008;
    stackFrame->r9=	0x009;
    stackFrame->r8= 0x00A;
    stackFrame->rsi= (uint64_t)argv;
    stackFrame->rdi= (uint64_t)argc;
    stackFrame->rbp= 0x00D;
    stackFrame->rdx= startingPoint;
    stackFrame->rcx= 0x00F;
    stackFrame->rbx= 0x010;
    stackFrame->rax= 0x011;
    stackFrame->rip= (void *) &runProcess;//(uint64_t)//startingPoint;
    stackFrame->cs=	0x008;
    stackFrame->eflags= 0x202;
    stackFrame->rsp= (uint64_t)&(stackFrame->base); //el rsp apunta a la direccion en la que esta el base
    stackFrame->ss= 0x000;
    stackFrame->base= 0x000;
}

void printStackFrame(tStackFrame * stackFrame) {
    putStr("Stack Frame ", colour2);
    putStr("rsi: ", colour2);
    uintToBase(stackFrame->rsi, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);

    putStr(" rdi: ", colour2);
    uintToBase(stackFrame->rdi, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);

    putStr(" rip: ", colour2);
    uintToBase(stackFrame->rip, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);

    putStr(" rsp: ", colour2);
    uintToBase(stackFrame->rsp, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);
}

void printProcess(tProcess * p) {
    putStr("Process ", colour2);
    putStr("pid: ", colour2);
    uintToBase(p->pid, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);
    putStr("name: ", colour2);
    putStr(p->name, colour2);
    putChar('\n', colour2);
    putStr("state: ", colour2);
    switch (p->state) {
        case READY:
            putStr("Ready", colour2);
            break;
        case RUNNING:
            putStr("Running", colour2);
            break;
        case WAITING:
            putStr("Waiting", colour2);
            break;
        case DEAD:
            putStr("Dead", colour2);
            break;
    }
    putChar('\n', colour2);
    putStr("stack pointer: ", colour2);
    uintToBase(p->stackPointer, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);

    putStr("memory low: ", colour2);
    uintToBase(p->processMemoryLowerAddress, buff2, 10);
    putStr(buff2, colour2);
    putChar('\n', colour2);

    printStackFrame(p->stackPointer);
}

// implementar yield para pasar a un proceso de ruuning a ready/waiting asi darle timepo a otro sin que este sea interrumpido por el timer tick

void endProcess(int pid) {
    //_cli();
    changeProcessState(pid, DEAD);
    _hlt();
    //runNextProcess();
}

void deleteProcess(tProcess* process) {
    freeMemory(process->processMemoryLowerAddress);
    freeProcessHeap(process->heap);
    //freeQueue(process->heap);
    freeMemory(process);
}

void freeProcessHeap(queueADT heap){
  while(heap!=NULL && (getSize(heap) > 0)){
    freeMemory(pop(heap)); //libera lo q quedo reservado y sin liberar y tamb el pop va borrando los nodos de la cola
  }
}


int cmpPointers(uint64_t  p1, uint64_t  p2) {
    return p1 - p2;
}

void* mallocMemoryInProcess(size_t request, tProcess* process){
  void* p = mallocMemory(request);
  if(process->heap == NULL){
      process->heap = newQueue(sizeof(uint64_t), cmpPointers);
  }

  // putStr("\ntamano cola heap antes: ", colour2);
  // uintToBase((process->heap)->dim, buff2, 10);
  // putStr(buff2, colour2);
  // putStr("\n", colour2);


  push(process->heap, p);
  process->memoryAllocated += request;
  //putStr("\nno la quedo en push", colour2);

  //TNode* aux = (process->heap)->first;

  // // putStr("\nse inserto: \n", colour2);
  // putStr("\ntamano cola heap despues: ", colour2);
  // uintToBase((process->heap)->dim, buff2, 10);
  // putStr(buff2, colour2);
  // putStr("\n", colour2);
  // // putStr("\ncola heap: ", colour2);


  // while(aux != NULL){
  //   putStr("M", colour2);
  //   uintToBase(aux->elem, buff2, 10);
  //   putStr(buff2, colour2);
  //   putStr("\n", colour2);
  //   aux = aux->next;
  // }
  //
  // putStr("\n", colour2);
return p;
}

void freeMemoryInProcess(void* memoryAdr, tProcess* process){
  // TNode* aux = (process->heap)->first;
  // putStr("\ntamano cola heap antes de borrar: ", colour2);
  // uintToBase((process->heap)->dim, buff2, 10);
  // putStr(buff2, colour2);
  // putStr("\n", colour2);
  //
  // while(aux != NULL){
  //   putStr("F", colour2);
  //   uintToBase(aux->elem, buff2, 10);
  //   putStr(buff2, colour2);
  //   putStr("\n", colour2);
  //   aux = aux->next;
  // }
    process->memoryAllocated -= size_of_level(findLevel(memoryAdr));
    uint64_t * aux = removeElem(process->heap, memoryAdr);
    freeMemory(aux);

  ///putStr("\nno la quedo en free mem: ", colour2);

  // putStr("\ntamano cola heap despues de borrar: ", colour2);
  // uintToBase((process->heap)->dim, buff2, 10);
  // putStr(buff2, colour2);
  // putStr("\n", colour2);
  //
  // aux = (process->heap)->first;
  //
  // while(aux != NULL){
  //   putStr("F", colour2);
  //   uintToBase(aux->elem, buff2, 10);
  //   putStr(buff2, colour2);
  //   putStr("\n", colour2);
  //   aux = aux->next;
  // }

}


void* callocMemoryInProcess(size_t request, tProcess* process){
  void* p = callocMemory(request);
  if(process->heap == NULL){
      process->heap = newQueue(sizeof(uint64_t), cmpPointers);
  }

  push(process->heap, p);

return p;
}

void* reallocMemoryInProcess(size_t request, tProcess* process, uint64_t oldPtr){
  removeElem(process->heap, oldPtr);
  void* p = reallocMemoryAlreadyFreed(oldPtr, request);
  if(process->heap == NULL){
      process->heap = newQueue(sizeof(uint64_t), cmpPointers);
  }

  push(process->heap, p);

return p;
}


int stateIdentifier(pState state){
  if(state == READY){
     return 0;
  }
  else if(state == RUNNING){
    return 1;
  }
  else if(state == WAITING){
    return 2;
  }
    return 3;

}
