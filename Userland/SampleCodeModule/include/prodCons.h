#ifndef prodCons_h
#define prodCons_h

#include "stdio.h"
#include "sync.h"
#include "stdlib.h"
#include "commands.h"
#include "VideoModule.h"
void initProdCons();
int myRand();
int myRandInRange(int min, int max);
void sushiMan();
void addToBuffer(char * sushi);
void printBuffer();
char * makeSushi();
void monster();
void eatFromBuffer();

#endif
