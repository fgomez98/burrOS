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

//esta bien este nombre?
tPipe pipe(char* name){
  tPipe p;
  _syscall(_pipe, name, &p);
  return p;
}

void unlinkPipe(char* name){
  _syscall(_unlinkPipe, name);
}

int readPipe(tPipe pipe, char* resp, int amount){
  int a;
  _syscall(_readPipe, &a, pipe, resp, amount);
  return a;
}

int writePipe(tPipe pipe, char * msg, int cant){
  int a;
  _syscall(_writePipe, &a, pipe, msg, cant);
  return a;
}
