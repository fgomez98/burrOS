#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <messagesDemo.h>
#include <sync.h>
#include <piping.h>
#include <String.h>

int fd[2];
tSem sem = 0;
tSem readSem = 0;
char * string = 0;

void makePiping(void (reader)(), void (writer)(char *), int argcant, char *argv[]) {
    if(sem == 0)
        sem = createSem("shell Control");
    if(readSem == 0)
        readSem = createSem("read sem");
    if(fd[0] < 10 || fd[1] < 11){
        fd[0] = 10;
        fd[1] = 11;
    }
    else {
        fd[0]+=2;
        fd[1]+=2;
    }
    char *argv1[1];
    argv1[0] = 0;

    string = argv[0];

    char *argv2[1];
    argv2[0] = 1;

    printf("\n");
    //reader
    exec("piping1", runProgram, writer, argv1);
    //writer
    exec("piping2", runProgram, reader, argv2);
    return;
}

void runProgram(void (f)(), char *argv[]) {
    pipe(fd);

    if(argv[0] == 0) {
        char *aux[2];
        aux[1] = argv[1];
        dup(fd[0],0);
        close(fd[1]);
        f(string);
        close(fd[0]);
    }
    else {
        dup(fd[1], 1);
        close(fd[0]);
        f(0);
        putChar(-1);
        close(fd[1]);
    }

    return;
}

void echoInput() {

    printf("\nIm reading from input!\n");
    char c = 1;
    while((c=getCharWithCero())!= -1){
        putChar(c);
    }
    printf("Reading is done!");
    giveControlToShell();
    return;
}

void findAndRemark(int argc, char *s[]) {
    if(argc == 1){
        string = malloc(50);
        string = strcpy(string,s[0]);
    }
    printf("\nI will remark the string: %s\n", string);
    char c = 1;
    char buffer[256];
    int i = 0;
    int aux = 0;
    while((c=getChar())!= -1){
        putChar(c);
        if (c == '\b') {
            if (i > 0)
                i--;
        } else {
            buffer[i++] = c;
            buffer[i] = 0;
            int j = 0;
            while(buffer[j] == string[j] && j < i) {
                j++;
            }
            if (string[j] == 0) {
                int del = 0;
                while (del < i) {
                    deleteChar();
                    del++;
                }
                printf("[(%s)]", buffer);
                i = 0;
            }
            else if(j < i){
                i = 0;
                if(c == string[0])
                    i++;
            }
        }

       /* if(c != '\b' && c != string[i]) {
            i = 0;
        }
        if(c == string[i]) {
            buffer[i++] = c;
            putChar(c);
            if(string[i] == 0) {
                buffer[i] = 0;
                int j = 0;
                while(j < i) {
                    deleteChar();
                    j++;
                }
                printf("[(%s)]", buffer);
                i = 0;
            }
        }
        else if (c == '\b') {
            if(i>0) {
                i--;
            }
            putChar(c);
        }
        else
            putChar(c);*/
    }
    if(argc == 1){
        free(string);
        string = 0;
    }
    printf("Reading is done!");

    giveControlToShell();
    return;
}
