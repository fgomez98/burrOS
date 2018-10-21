#ifndef sync_h
#define sync_h
#include <stdint.h>
#include "stdio.h"
#include "syscall.h"

typedef uint64_t* tMutex;
typedef uint64_t* tSem;
typedef uint64_t* tPipe;
tMutex createMutex(char* mutexName);
void destroyMutex(tMutex mutex);
void adquire(tMutex mutex);
void release(tMutex mutex);
tSem createSem(char* semName);
void destroySemaphore(tSem sem);
void wait(tSem sem);
void post(tSem sem);
tPipe namedPipe(char* name);
void unlinkPipe(char* name);
int readPipe(tPipe pipe, char* resp, int amount);
int writePipe(tPipe pipe, char * msg, int amount);
#endif
