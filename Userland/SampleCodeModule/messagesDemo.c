#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <messagesDemo.h>
#include <sync.h>


void startMessagesDemo() {
    writeWelcomeMessage();
    char * pipeName = "thisIsADemoPipe";
    char c;
    while((c=getChar()) != 'q') {
        switch(c) {
            case '1': {
                printf("Your input below: \n");
                char input[50];
                int i = 0;
                while((c=getChar()) != '\n') {
                    input[i] = c;
                    printf("%c",c);
                    i++;
                }
                if(i == 0){
                    printf("Nothing to write\n");
                    break;
                }
                printf("\n");
                i++;
                char * argv[2];
                argv[0] = pipeName;
                argv[1] = input;
                exec("escribe1",writeMessage,i,argv);
                break;
            }
            case '2': {
                int numberToRead = 0;
                for(int pot= 1;(c=getChar()) >= '0' && c <= '9'; pot*=10) {
                    numberToRead = (numberToRead * pot) + (c - '0');
                }
                char * argv[1];
                argv[0] = pipeName;
                exec("lee1",readMessage,numberToRead,argv);
                break;
            }
        }
    }
}

void writeMessage(int argc, char ** argv) {
    tPipe myPipe = pipe(argv[0]);
    writePipe(myPipe,argv[1], argc);
    argv[1][argc] = '\0';
    printf("I wrote %s\n", argv[1]);
    return;
}

void readMessage(int argc, char ** argv) {
    tPipe myPipe = pipe(argv[0]);
    char buffer[argc];
    readPipe(myPipe, buffer,argc);
    buffer[argc] = '\0';
    printf("I read: %s", buffer);
    return;
}

void writeWelcomeMessage() {
    printf("\n1: Write whatever you write until you press enter.(MAX 50 chars)\n");
    printf("2: Read and print a number of bytes.\n");
    printf("q: Exit demo\n");
}
