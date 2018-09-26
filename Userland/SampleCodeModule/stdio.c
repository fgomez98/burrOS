#include "stdio.h"
#include "lib.h"
#include "syscall.h"
#define WRITE 1
#define READ 0
#define STDOUT 1
#define STDIN 0
#define CLEAR 0

static char buffer[MAXLENGTH];

void scanAndPrint(char* buffer) {

  char k;
  int i=0;

  while(((k = getChar()) != '\n')  && i< MAXLENGTH - 1){
    if(k == '\b'){
        if (i > 0) {
            _syscall(_deleteChar);
            i--;
        }
    }
    else if (k > 0 && k < 127) {
      _syscall(_putChar, k);
      buffer[i++] = k;
    }

  	}
    buffer[i]=0;

  }

void deleteChar() {
    _syscall(_deleteChar);
}

void putChar(char c) {
    _syscall(_putChar, c);
}

void putString(char * str) {
    _syscall(_putString, str);
}


char getChar(){
  char c=0;
	while(1) {
        _syscall(_read, &c);
        if ( c > 0 && c <128) {
            return c;
        }
	}
  return 0;
}


void printf(char* fmt, ...) {

    va_list args;
    va_start(args, fmt);

    int i;
    char* s;
    char printable[64];

    while(*fmt){
        if(*fmt != '%'){
            putChar(*fmt);
        }else{
          fmt++;
          switch(*fmt){
              case 'c':
                  i = (char) va_arg(args, int);
                  putChar((char)i);
                  break;
              case 'd':
                  i = va_arg(args, int);
                  uintToBase(i, printable, 10);
                  putString(printable);
                  break;
              case 's':
                  s = (char*) va_arg(args, char*);
                  putString(s);
                  break;
              case 'o':
                  i = va_arg(args, int);
                  uintToBase(i, printable, 8);
                  putString(printable);
                  break;
              case 'x':
                  i = va_arg(args, int);
                  uintToBase(i, printable, 16);
                  putString(printable);
                  break;
            }
        }
        fmt++;
    }
    va_end(args);
}


int scanf(const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  getInput(buffer);

  char * str = buffer;
  int i = 0;
  char* c;

  while(*fmt){
    if(*fmt != '%'){
      if((*fmt) != (*str)){
        return i;
      } else {
        fmt++;
        str++;
      }
    }else{
      fmt++;
      switch(*fmt){
        case 'd': str = getInt(str, va_arg(args, int));
                  i++;
                  break;
        case 's': c = getString(str, va_arg(args, char*));
                  i++;
                  break;
        case 'c': c = va_arg(args,char*);
                  *c = *str++;
                  i++;
                  break;
      }
    }
  }
  return i;
}

char * getInt(const char * str, int * n){
  while (!isNum(*str) && !(*str == '-' && isNum(*(str + 1)))){
    str++;
  }
  *n = atoi(str);
  while(*str && *str != ' '){
    str++;
  }
  return str;
}


char* getString(const char * str, char* source){
  while(*str){
    *source = *str;
    *source++;
    *str++;
  }
  return str;
}

void getInput(char * string){
  char c;
  int i=0;

  while((c = getChar()) != '\n'){
    putChar(c);
    if(i < MAXLENGTH && (c != '\b' && c != '$') && (c > 0 && c < 127)){
      string[i++] = c;
    }
    if(i == MAXLENGTH-1){
      printf("MAX BUFFER CAPACITY EXCEEDED");
    }
  }
  string[i] = '\0';
}
