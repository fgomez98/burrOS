#ifndef sync_h
#define sync_h
#include <stdint.h>
#include "stdio.h"
#include "systemCall.h"

typedef uint64_t* tMutex;
tMutex createMutex(char* mutexName);
void destroyMutex(tMutex mutex);
void adquireMutex(tMutex mutex);
void releaseMutex(tMutex mutex);

#endif
