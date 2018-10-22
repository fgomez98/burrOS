#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <messagesDemo.h>
#include <sync.h>

int fd;

void startMessagesDemo() {
    writeWelcomeMessage();
    open(fd);
    char * pipeName = "thisIsADemoPipe";
    char c;
    while((c=getChar()) != 'q') {
        switch(c) {
            case '1': {
                printf("Your input below: \n");
                char input[50];
                int i = 0;
                while((c=getChar()) != '\n') {
                    if(c == '\b') {
                        if(i > 0) {
                            i--;
                            printf("%c", c);
                        }
                    }
                    else {
                        input[i] = c;
                        printf("%c", c);
                        i++;
                    }
                }
                if(i == 0){
                    printf("Nothing to write\n");
                    break;
                }
                printf("\n");
                char * argv[2];
                argv[0] = pipeName;
                argv[1] = input;
                exec("escribe1",writeMessage,i,argv);
                break;
            }
            case '2': {
                int numberToRead = 0;
                printf("Your input below: \n");
                for(int pot= 1;(c=getChar()) >= '0' && c <= '9'; pot*=10) {
                    numberToRead = (numberToRead * pot) + (c - '0');
                    printf("%c",c);
                }
                if(numberToRead == 0){
                    printf("Nothing to read\n");
                    break;
                }
                printf("\n");
                char * argv[1];
                argv[0] = pipeName;
                exec("lee1",readMessage,numberToRead,argv);
                break;
            }
        }
    }
}

void writeMessage(int argc, char ** argv) {
    tPipe myPipe = namedPipe(argv[0]);
    open(fd);
    write(fd,argv[1], argc);
    argv[1][argc] = '\0';
    printf("I wrote %s\n", argv[1]);
    killCurrentProcess();
}

void readMessage(int argc, char ** argv) {
    tPipe myPipe = namedPipe(argv[0]);
    open(fd);
    int amount = argc;
    char buffer[amount];
    int a = readfd(fd, buffer,amount);
    buffer[a] = '\0';
    printf("I read %d bytes: %s\n",a, buffer);
    killCurrentProcess();
}

void writeWelcomeMessage() {
    printf("\n1: Write in the pipe whatever you write. Press enter to finish your message.(MAX 50 chars)\n");
    printf("2: Read and print a number of bytes. Type the number, then press enter.\n");
    printf("q: Exit demo\n");
}
