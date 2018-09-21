#include "stdlib.h"
#include "stdio.h"
#include "systemCall.h"

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
