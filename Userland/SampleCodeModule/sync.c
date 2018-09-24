#include "sync.h"

tMutex createMutex(char* mutexName){
  tMutex mutex;
  systemCall(10, mutexName, &mutex,0,0,0);
	return mutex;
}


void destroyMutex(tMutex mutex){
	systemCall(11, mutex, 0, 0, 0,0);
}

void adquireMutex(tMutex mutex){
  systemCall(12, mutex, 0,0,0,0);
}

void releaseMutex(tMutex mutex){
  systemCall(13, mutex, 0,0,0,0);
}
