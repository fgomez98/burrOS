#include "sync.h"


tMutex createMutex(char* mutexName){
  tMutex mutex;
  _syscall(_createMutex, mutexName, &mutex);
	return mutex;
}


void destroyMutex(tMutex mutex){
  _syscall(_destroyMutex, mutex);
}

void adquire(tMutex mutex){
  _syscall(_adquireMutex, mutex);
}

void release(tMutex mutex){
  _syscall(_releaseMutex, mutex);
}

tSem createSem(char* semName) {
    tSem sem;
    _syscall(_createSem, semName, &sem);
    return sem;
}

void destroySemaphore(tSem sem) {
    _syscall(_destroySem, sem);

}

void wait(tSem sem) {
    _syscall(_semWait, sem);
}

void post(tSem sem) {
    _syscall(_semPost, sem);
}
