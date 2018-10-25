#include <syscall.h>
#include <stdio.h>
#include "stdlib.h"
#include <pipesDemo.h>
#include <sync.h>

int fd1[2];
int fd2[2];
int running;

void initPipesDemo(){
  running = 1;
  welcomeMessage();

  exec("messenger1",messenger1,0,0);
  exec("messenger2",messenger2,0,0);

  while (running){
    char c;
    _syscall(_read, &c );
    if(c == 'q'){
      running = 0;
    }
  }
}

void messenger1(){
  fd1[0] = 40;
  fd1[1] = 900;
  fd2[0] = 41;
  fd2[1] = 901;

  pipe(fd1);
  pipe(fd2);

  int message = "Messenger 1 says hello!\n" ;
  while (running){
    printf("Sending hi to messenger 2!\n");
    write(fd1[1], message,22);
    printf("Opening mailbox of process 1\n");
    char buffer[23];
    delay(600);
    readfd(fd2[0], buffer,22);
    buffer[22] = '\0';
    printf("\nThe message is: %s, and i am in 1\n", buffer);
  }
}

void messenger2(){
  fd1[0] = 40;
  fd1[1] = 900;
  fd2[0] = 41;
  fd2[1] = 901;

  pipe(fd1);
  pipe(fd2);

  int message = "Messenger 2 says hello!\n" ;
  while (running){
    printf("Sending hi to messenger 1!\n");
    write(fd2[1], message,22);
    printf("Opening mailbox of process 2\n");
    char buffer[23];
    delay(6000);
    readfd(fd1[0], buffer,22);
    buffer[22] = '\0';
    printf("\nThe message is: %s, and i am in 2\n", buffer);
  }
}

void welcomeMessage(){
  printf("\nWelcome to the pipes demostration, in this application we will establish two simple processes that use a pair of pipes to send messages to each other \n");
  printf("Press q at anytime to quit\n");
  printf("Process will start shortly...\n");
  delay(30000);
}
