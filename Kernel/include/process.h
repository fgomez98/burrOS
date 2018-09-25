#ifndef PROCESS_H
#define PROCESS_H

#include "stdint.h"
#include "BuddyAllocationSystem.h"
#include "lib.h"
#include "queueADT.h"
#include "String.h"

#define BLOCK_SIZE 4096
#define PROCESS_SIZE 2*BLOCK_SIZE
#define MAX_MESSAGES 5
#define MESSAGE_SIZE 500
typedef enum {READY, RUNNING, WAITING, DEAD} pState; //estados de un proceso

//MESSAGING

typedef struct message_buffer{
  int source;
  long type;
  char * text;
}tMessage;

typedef struct mail_box{
  tMessage * buffer[MAX_MESSAGES];
  int messages;
  queueADT * waitQueue;
  struct mutex * send;
}tMailBox;

typedef struct{
    int pid; //process ID
    int parentPid; //parent's ID
    char* name; //process name
    pState state; //process status
    void * stackPointer;
    void * processMemoryLowerAddress;
    queueADT heap;
    uint64_t memoryAllocated;
    tMailBox * mailBox;
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
//void initializeStack(tStackFrame* stackPointer, int argc, char* argv[], void* startingPoint);
void* initializeStack(void * stackPointer, int argc, char* argv[], void * startingPoint);
void* mallocMemoryInProcess(size_t request, tProcess* running);
int cmpPointers(uint64_t  p1, uint64_t  p2) ;
void freeMemoryInProcess(void* memoryAdr, tProcess* running);
void freeProcessHeap(queueADT heap);
void endProcess(int pid);
int stateIdentifier(pState state);
void* callocMemoryInProcess(size_t request, tProcess* process);
void* reallocMemoryInProcess(size_t request, tProcess* process, uint64_t oldPtr);
tMessage * getMessage(tProcess * p);
void sendMessage(int pid, tMessage * message, tProcess * source);
void setProcessMailBox(tProcess * process);
tMessage * createMessage(int process, long type, char * text);

#endif
