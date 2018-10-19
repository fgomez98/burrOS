#include "shell.h"
#include "commands.h"
#include "stdio.h"
#include "stdlib.h"
#include "messagesDemo.h"
#include "VideoModule.h"
#include "prodCons.h"

void probando(){
    while(1);
}

Colour white = {255, 255, 255};

void initializeShell() {

    showBurro();
    printf("Welcome to the shell!! Please type help to get a list of our commands\n");

  static char command[MAXLENGTH];

 

  int running = 1;
  while (running){
    char * arg1 = calloc(MAXLENGTH);
    char * arg2 = calloc(MAXLENGTH);
    printf("$>");
    scanAndPrint(command);


      sscanf("%s & %s", command, arg1, arg2);
      //TODO: Un semaforo para que se ejecute uno despues del otro??
      if (*arg2) {
     if(strcmp("help", arg2) == 0){
         exec("help",help, 0, 0);
     }
     else if(strcmp("digitalTime", arg2) == 0){
         exec("showDigitalHour",showDigitalHour, 0, 0);
     }
     else if(strcmp("time", arg2) == 0){
         exec("showTime",showTime, 0, 0);
     }
     else if(strcmp("clear", arg2) == 0){
         exec("clearScreen",cleanScreen, 0, 0);
     }
     else if(strcmp("zero division", arg2) == 0){
         exec("zeroDiv",divi, 0, 0);
     }
     else if(strcmp("invalid opcode", arg2) == 0){
         exec("opcodeExc",showOpcodeExc, 0, 0);
     }
     else if(strcmp("exit", arg2) == 0){
         running = 0;
     }
     else if(strcmp("ps", arg2) == 0){
         exec("ps",ps, 0, 0);
     }
     else if(strcmp("memory", arg2) == 0){
         exec("memory",memory, 0, 0);
     }
     else if(strcmp("exec", arg2) == 0){
         int pid = exec("probando",probando, 0, 0);
     }
     else if(strcmp("malloc", arg2) == 0){
         maDemo();
         // char* a = malloc(10);
         // char *b = malloc(20);
         // free(a);
         // free(b);
     } else if (strcmp("sushi", arg2) == 0) {
         exec("initProdCons",initProdCons, 0, 0);
     } else if (strcmp("necesitoquemeapapachen", arg2) == 0) {
         exec("burro",showBurro, 0, 0);
     } else if (strcmp("backgroundTest", arg2) == 0) {
         exec("backgroundTest", stayAlive, 0, 0);
     } else{
         printf("\nUnknown command, type help\n");
         continue;
     }
 } if (*arg1) {
     if(strcmp("help", arg1) == 0){
         help();
     }
     else if(strcmp("digitalTime", arg1) == 0){
         showDigitalHour();
     }
     else if(strcmp("time", arg1) == 0){
         showTime();
     }
     else if(strcmp("clear", arg1) == 0){
         cleanScreen();
     }
     else if(strcmp("zero division", arg1) == 0){
         divi();
     }
     else if(strcmp("invalid opcode", arg1) == 0){
         showOpcodeExc();
     }
     else if(strcmp("exit", arg1) == 0){
         running = 0;
     }
     else if(strcmp("ps", arg1) == 0){
        // exec("probando",probando, 0, 0);
        exec("ps",ps, 0, 0);
         //ps();
     }
     else if(strcmp("memory", arg1) == 0){
         memory();
     }
     else if(strcmp("exec", arg1) == 0){
         int pid = exec("probando",probando, 0, 0);
     }
     else if(strcmp("malloc", arg1) == 0){
         maDemo();
     } else if (strcmp("sushi", arg1) == 0) {
         initProdCons();
     } else if (strcmp("necesitoquemeapapachen", arg1) == 0) {
         showBurro();
     } else if (strcmp("backgroundTest", arg1) == 0) {
         exec("backgroundTest", stayAlive, 0, 0);
     } else{
         printf("\nUnknown command, type help\n");
         continue;
     }
 }

    free(arg1);
    free(arg2);
  }
   printf("\n\n\nSee you soon!");
  
}
