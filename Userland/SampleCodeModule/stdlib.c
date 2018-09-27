#include "stdlib.h"
#include "syscall.h"

int atoi(char * str){
  int res = 0;
  int sign = 1; //positive
  int i = 0;

  if(str[0] == '-'){
    sign = -1;
    i = 1;
  }

  for(; str[i] != '\0' && isNum(str[i]); i++){
    res = res*10 + str[i] - '0';
  }

  return sign*res;
}

int isNum(char c){
  return (c >= '0' && c <= '9');
}

int isAlpha(char c){
  return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isSpace(char c){
  return c == ' ';
}

int isSuper(char c){
  return (c >= 'A') && (c <= 'Z');
}


void * malloc(int bytes){
    void* address;
    _syscall(_malloc, bytes, &address);
    return address;
}

void * calloc(int bytes) {
    void* address;
    _syscall(_calloc, bytes, &address);
    return address;
}

void * realloc(void* ptr, int bytes){
    void* address;
    _syscall(_realloc, bytes, &address, ptr);
    return address;
}

void free(void* ad){
  _syscall(_free, ad);
}

int exec(char* name,void* startingPoint, int argc, void* argv[]) {
    int pid;
    _syscall(_exec,name , startingPoint, &pid, argc, argv);
    return pid;
}

/*
  msg:
    0: ends process
    1: blocks process
    2: unblocks process
*/
void kill(int pid, int msg) {
  _syscall(_kill, pid, msg);
}

//kill q mata al current process
void killCurrentProcess() {
  _syscall(_killCurrentProcess);
}
