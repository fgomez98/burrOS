#include "sync.h"

tMutex createMutex(char* mutexName){
  tMutex mutex;
  systemCall(10, mutexName, &mutex,0,0,0);
	return mutex;
}


void destroyMutex(tMutex mutex){
	systemCall(11, mutex, 0, 0, 0,0);
}

void adquire(tMutex mutex){
  systemCall(12, mutex, 0,0,0,0);
}

void release(tMutex mutex){
  systemCall(13, mutex, 0,0,0,0);
}

tSem createSem(char* semName) {
    tSem sem;
    systemCall(14, semName, &sem,0,0,0);
    return sem;
}

void destroySemaphore(tSem sem) {
    systemCall(15, sem, 0, 0, 0,0);
}

void wait(tSem sem) {
    systemCall(16, sem, 0, 0, 0,0);
}

void post(tSem sem) {
    systemCall(17, sem, 0, 0, 0,0);
}