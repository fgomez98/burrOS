#include "shell.h"
#include "commands.h"
#include "stdio.h"
#include "stdlib.h"
#include "messagesDemo.h"
#include "VideoModule.h"
#include "prodCons.h"

void probando(){
  printf("Exec\n");
  killCurrentProcess();
}

Colour white = {255, 255, 255};
void initializeShell(){

  showBurro();
  printf("Welcome to the shell!! Please type help to get a list of our commands\n");

    static char command[MAXLENGTH];
  int running = 1;
  while (running){

  printf("$>");
  scanAndPrint(command);


    if(strcmp("help", command) == 0){
        help();
    }
    else if(strcmp("digitalTime", command) == 0){
      showDigitalHour();
    }
    else if(strcmp("time", command) == 0){
      showTime();
    }
    else if(strcmp("clear", command) == 0){
      cleanScreen();
    }
    else if(strcmp("zero division", command) == 0){
      divi();
    }
    else if(strcmp("invalid opcode", command) == 0){
      showOpcodeExc();
    }
    else if(strcmp("exit", command) == 0){
      running = 0;
    }
    else if(strcmp("ps", command) == 0){
      ps();
    }
    else if(strcmp("memory", command) == 0){
      memory();
    }
    else if(strcmp("exec", command) == 0){
      int pid = exec(probando, 0, 0);
    }
    else if(strcmp("malloc", command) == 0){
      // char* a = malloc(10);
      // char *b = malloc(20);
      // free(a);
      // free(b);
    }
    else if (strcmp("sushi", command) == 0) {
        initProdCons();
    }
    else if (strcmp("necesito que me apapachen", command) == 0) {
        showBurro();
    }
    else if(strcmp("m", command) == 0) {
        startMessagesDemo();
    }
    else if(strcmp("MSN", command) == 0){
      startProcessCommunication();
    }
    else{
      printf("\nUnknown command, type help\n");
    }


  }
   printf("\n\n\nSee you soon!");

}
