#ifndef _defs_
#define _defs_
#include "process.h"
#include "queueADT.h"
#include <stdint.h>
#include "lib.h"
#include <VideoDriver.h>
#define MAX_PROCESSES 20



typedef struct processNode{
  tProcess* process;
  struct processNode * next;
} tProcessNode;

int getRunningPid();
void addProcess(tProcess * p);
void getFromWaiting();
void blockProcess(int pid);
void unblockProcess(int pid);
void runNextProcess();
//tProcess * removeProcess(tProcess * p);
void scheduler();
void * dispatcher(int rsp);
void init_Process();
void init_(void * startingPoint);
int cmpProcess(tProcess * p1, tProcess * p2);
tProcess* getRunningProcess();
void mutexTest();

#endif
