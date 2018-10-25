#include "stdio.h"
#include "lib.h"
#include <string.h>
#include "syscall.h"
#define WRITE 1
#define READ 0
#define STDERR 2
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
      putChar(k);
      buffer[i++] = k;
    }

  	}
    buffer[i]=0;

  }

void deleteChar() {
    _syscall(_deleteChar);
}

void putStringWithSize(char * str, int size){
    write(STDOUT, str, size);
}

void putChar(char c) {
    write(STDOUT, &c,1);
   //_syscall(_putChar, c);
}

void putString(char * str) {
    write(STDOUT, str, strlen(str));
 //   _syscall(_putString, str);
}

int nice(pid, priority) {
    return _syscall(_nice, pid, priority);
}

char getChar(){
  char c=0;
	while(1) {
        readfd(0,&c,1);
        if ( c > 0 && c <128) {
            return c;
        }
	}
  return 0;
}

static
void putStringToBuffer(char * buffer, char * message, int * index) {
    for(int i = 0; message[i] != '\0'; i++, (*index)++){
        buffer[*index] = message[i];
    }
}


void printf(char* fmt, ...) {

    va_list args;
    va_start(args, fmt);

    int i;
    char* s;
    char printable[64];
    char bufferPrint[2048];
    int j = 0;
    while(*fmt){
        if(*fmt != '%'){
            bufferPrint[j++] = *fmt;
        }else{
            fmt++;
            switch(*fmt){
                case 'c':
                    i = (char) va_arg(args, int);
                    bufferPrint[j++] = (char) i;
                    break;
                case 'd':
                    i = va_arg(args, int);
                    uintToBase(i, printable, 10);
                    putStringToBuffer(bufferPrint,printable,&j);
                    break;
                case 's':
                    s = (char*) va_arg(args, char*);
                    putStringToBuffer(bufferPrint,s,&j);
                    break;
                case 'o':
                    i = va_arg(args, int);
                    uintToBase(i, printable, 8);
                    putStringToBuffer(bufferPrint,printable,&j);
                    break;
                case 'x':
                    i = va_arg(args, int);
                    uintToBase(i, printable, 16);
                    putStringToBuffer(bufferPrint,printable,&j);
                    break;
            }
        }
        fmt++;
    }
    bufferPrint[j] = '\0';
    putStringWithSize(bufferPrint, j);
    va_end(args);
}


int scanf(const char* fmt, ...){
  va_list args;
  va_start(args, fmt);
  getInput(buffer);

  char * str = buffer;
  int i = 0;
  char* c;

  while(*fmt!='\0'){
    if(*fmt != '%'){
      if((*fmt) != (*str)){
        return i;
      } else {
        fmt++;
        str++;
      }
    } else{
      fmt++;
      switch(*fmt){
        case 'd': str = getInt(str, va_arg(args, int));
                  i++;
                  break;
        case 's': c = getString(str, va_arg(args, char*));
                  printf("skere");
                  i++;
                  break;
        case 'c': c = va_arg(args,char*);
                  *c = *str++;
                  i++;
                  break;
      }
      ++fmt; //este no se si lo rompe
    }
  }
  return i;
}


int sscanf(const char * fmt, const char * str, ...) {
    va_list args;
    va_start( args, fmt );
    char strnum[10];
    char * c;
    int i = 0;
    while(*fmt != '\0'){
        if(*fmt != '%'){
            if((*fmt) != (*str)){
                return i;
            }else{
                fmt++;
                str++;
            }
        } else{
            switch(*(++fmt)){
                case '%':
                    if (*str != '%') {
                        return i;
                    } else {
                        str++;
                    }
                    break;
                case 'd': c = str;
                    str = getInt(str, va_arg(args,int));
                    if(str != c){
                        i++;
                    }
                    break;
                case 'c': c = va_arg(args, char*);
                    *c = *str++;
                    i++;
                    break;
                case 's': c = va_arg(args, char*);
                    while( *str &&  *str!= ' '){
                        *c = *str;
                        c++;
                        str++;
                    }
                    i++;
                    break;
            }
            ++fmt;
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

char* getString(const char * str, char*  dest){
  int i = 0;
  while(str[i] && str[i] != ' ' ){
    printf("%c\n",*str );
    dest[i]=str[i];
    i++;

  }
  return dest;
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
