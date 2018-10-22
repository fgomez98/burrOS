#ifndef PROCESS_H
#define PROCESS_H

#include "stdint.h"
#include "BuddyAllocationSystem.h"
#include "lib.h"
#include "queueADT.h"
#include "String.h"
#include "linkedList.h"

#define BLOCK_SIZE 4096
#define PROCESS_SIZE 2*BLOCK_SIZE

typedef enum {READY, RUNNING, WAITING, DEAD} pState; //estados de un proceso



//typedef struct{
//    int pid; //process ID
//    int parentPid; //parent's ID
//    char* name; //process name
//    pState state; //process status
//    void * stackPointer;
//    void * processMemoryLowerAddress;
//    queueADT heap;
//    uint64_t memoryAllocated;
//} tProcess;

typedef struct {
    int pid; //process ID
    int parentPid; //parent's ID
    int priority;
    int quantumTime;
    char * name; //process name
    pState state; //process status
    void * stackPointer;
    void * processMemoryLowerAddress;
    void * code;
    queueADT heap;

    queueADT fdList;
//    queueADT mutexs; // la idea es que si tiene algun ipc adquirido aumentarle la prioridad a este asi libera el recurso mas rapido
//    queueADT semaphores;

    uint64_t memoryAllocated;

} tProcess;

//RowDaBoat
typedef struct {
    uint64_t gs;
    uint64_t fs;
    uint64_t r15;
    uint64_t r14;
    uint64_t r13;
    uint64_t r12;
    uint64_t r11;
    uint64_t r10;
    uint64_t r9;
    uint64_t r8;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rdx;
    uint64_t rcx;
    uint64_t rbx;
    uint64_t rax;

    uint64_t rip;
    uint64_t cs;
    uint64_t eflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t base;
} tStackFrame;


tProcess* createProcess(char* processName,void* startingPoint, int parentPid, int argc, char* argv[]);
void deleteProcess(tProcess* process);
void initializeStack(tStackFrame* stackPointer, int argc, char* argv[], void* startingPoint);
//void* initializeStack(void * stackPointer, int argc, char* argv[], void * startingPoint);
void* mallocMemoryInProcess(size_t request, tProcess* running);
int cmpPointers(uint64_t  p1, uint64_t  p2) ;
void freeMemoryInProcess(void* memoryAdr, tProcess* running);
void freeProcessHeap(queueADT heap);
void endProcess(int pid);
int stateIdentifier(pState state);
void* callocMemoryInProcess(size_t request, tProcess* process);
void* reallocMemoryInProcess(size_t request, tProcess* process, uint64_t oldPtr);
void addFdToProcess(int fd);
void dup2(int newFd, int fdToReplace);

#endif
