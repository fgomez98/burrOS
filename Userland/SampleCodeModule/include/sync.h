#ifndef sync_h
#define sync_h
#include <stdint.h>
#include "stdio.h"
#include "syscall.h"

typedef uint64_t* tMutex;
typedef uint64_t* tSem;
tMutex createMutex(char* mutexName);
void destroyMutex(tMutex mutex);
void adquire(tMutex mutex);
void release(tMutex mutex);
tSem createSem(char* semName);
void destroySemaphore(tSem sem);
void wait(tSem sem);
void post(tSem sem);

#endif
