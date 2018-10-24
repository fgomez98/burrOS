#include "sync.h"
int fd[2];
typedef void (*functiontype)();
void runProgram(int argc, char ** argv);

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
tPipe namedPipe(char* name){
  tPipe p;
  _syscall(_pipe, name, &p);
  return p;
}

void destroyPipe(char* name){
    _syscall(_destroyPipe, name);
}

int readPipe(tPipe pipe, char* resp, int amount){
    int a;
    return _syscall(_readPipe, pipe, &a, resp, amount);
}

int writePipe(tPipe pipe, char * msg, int cant){
    int a;
    return _syscall(_writePipe, pipe, &a, msg, cant);
}

int open(int fd){
    return _syscall(_open, fd);
}

int write(int fd, char * msg, int amount){
    return _syscall(_write, fd,msg,amount);
}

int readfd(int fd, char * msg, int amount){
    return _syscall(_readfd, fd,msg,amount);
}

int close(int fd){
    return _syscall(_close, fd);
}

int pipe(int fd[]) {
    return _syscall(_pipe, fd);
}

void dup(int newFd, int oldFd) {
    _syscall(_dup, newFd,oldFd);
}

void makePiping(void * startingPoint1, void * startingPoint2) {
    int fd[2];
    if(fd[0] < 10 || fd[1] < 11){
        fd[0] = 10;
        fd[1] = 11;
    }
    char * argv1[1];
    argv1[0] = 0;
    char * argv2[1];
    argv2[0] = 1;
    exec("piping1",runProgram, startingPoint2, argv1);
    exec("piping2",runProgram, startingPoint1, argv2);
    return;
}

void runProgram(int argc, char ** argv) {
    pipe(fd);
    if(argv[0] == 0) {
        dup(fd[0],0);
    }
    else {
        dup(fd[1], 1);
    }
    functiontype f = (argc);
    f();
    return;
}





