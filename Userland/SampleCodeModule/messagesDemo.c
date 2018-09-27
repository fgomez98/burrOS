#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <messagesDemo.h>
#include <sync.h>

char * pipeName = "thisIsADemoPipe";

void startMessagesDemo() {
    writeWelcomeMessage();
    tPipe myPipe = pipe(pipeName);
    printf("%d\n", myPipe);
    char * string = "Hello";
    writePipe( myPipe, string, 5);
    char asd[50];
    readPipe(myPipe, asd, 5);
    asd[5] = '\0';
    printf("%s",asd);
}

void readAndPrint10() {
    tPipe pipe = _syscall(_pipe, pipeName);
    char * resp = malloc(11);
    readPipe( pipe, resp, 10);
    resp[10] = '\0';
    printf("I read: %s", resp);
    killCurrentProcess();
}

void readAndPrint5() {
    tPipe pipe = _syscall(_pipe, pipeName);
    char * resp = malloc(6);
    readPipe( pipe, resp, 5);
    resp[5] = '\0';
    printf("I read: %s", resp);
    killCurrentProcess();
}

void writeMessage1() {
    tPipe * pipe = _syscall(_pipe, pipeName);
    char * string = "Hello";
    writePipe( pipe, string, strlen(string));
    printf("I wrote: %s\n", string);
    killCurrentProcess();
}

void writeMessage2() {
    tPipe pipe = _syscall(_pipe, pipeName);
    char * string = "This is a demo";
    writePipe( pipe, string, strlen(string));
    printf("I wrote: %s", string);
    killCurrentProcess();
}

void writeWelcomeMessage() {
    printf("\n1: Write 'Hello' on the pipe\n");
    printf("2: Read and print 5 chars from the pipe\n");
    printf("3: Write 'This is a demo' on the pipe\n");
    printf("4: Read and print 10 chars from the pipe\n");
    printf("q: Exit demo\n");
}
