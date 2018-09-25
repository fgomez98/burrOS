#include "stdlib.h"
void killCurrentProcess();

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


void* malloc(int bytes){
    void* address;
    systemCall(3, bytes, &address, 0,0,0 );
    return address;
}

void* calloc(int bytes){
    void* address;
    systemCall(14, bytes, &address, 0,0,0 );
    return address;
}

void* realloc(void* ptr, int bytes){
    void* address;
    systemCall(15, bytes, &address, ptr,0,0 );
    return address;
}

void free(void* ad){
  systemCall(4, ad,0,0,0,0);
}

int exec(void* startingPoint, int argc, void* argv[]) { // los parametros no se bien todavia
  int pid;
  systemCall(5, startingPoint, &pid, argc, argv, 0);
  return pid;
}

/*
  msg:
    0: ends process
    1: blocks process
    2: unblocks process
*/
void kill(int pid, int msg) {
  systemCall(6, pid, msg, 0,0,0);
}

//kill q mata al current process
void killCurrentProcess() {
  systemCall(7, 0, 0, 0,0,0);
}
