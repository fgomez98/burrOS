#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <messagesDemo.h>
#include <sync.h>
#include <piping.h>

int fd[2];
typedef void (*functiontype)();

void makePiping(void * startingPoint1, void * startingPoint2) {
    if(fd[0] < 10 || fd[1] < 11){
        fd[0] = 10;
        fd[1] = 11;
    }
    else {
        fd[0]+=2;
        fd[1]+=2;
    }
    char * argv1[1];
    argv1[0] = 0;
    char * argv2[1];
    argv2[0] = 1;
    exec("piping1", runProgram, readProgram, argv1);
    exec("piping2", runProgram, writeProgram, argv2);
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

void readProgram() {
    char c;
    char message[20];
    while((c=getChar())) {
        printf("%c", c);
    }
    return;
}

void writeProgram() {



    printf("hola como estaas");

    return;
}