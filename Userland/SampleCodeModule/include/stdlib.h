#ifndef lib_h
#define lib_h
#include <stdint.h>
#include "stdio.h"
#include "systemCall.h"

/*Recieves a char array containing anumber and returns said number as an integer(base 10)*/
int atoi(char * str);

/*Returns 1 if c is a number, else it returns 0*/
int isNum(char c);

/*Returns 1 if c is a letter(Uppercase or Lowercase), else it returns 0*/
int isAlpha(char c);

/*Returns 1 if c is a space, else it returns 0*/
int isSpace(char c);

/*Returns 1 if c is an Uppercase letter, else it returns 0*/
int isSuper(char c);

void* malloc(int bytes);
void free(void* ad);
void kill(int pid, int msg);


int exec(void* startingPoint, int argc, void* argv[]);
/*
 msg:
 0: ends process
 1: blocks process
 2: unblocks process
 */
void kill(int pid, int msg);

//kill q mata al current process
void killCurrentProcess();
#endif
