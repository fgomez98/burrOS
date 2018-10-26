#include "commands.h"
#include "stdio.h"
#include <stdint.h>
#include "stdlib.h"
#include "VideoModule.h"
#include "TimeModule.h"
#include "syscall.h"

extern void opcodeExc();

//static const int mycolours[SIZE] = {12354, 658456, 736534, 12444, 0xFF};
Colour colour1 = {255, 255, 255};
Colour colour2 = {180, 40, 18};
Colour colour3 = {200, 156, 12};
Colour colour4 = {233, 80, 167};
Colour colour5 = {46, 230, 210};

void help(){
    printf("\nHELP:\n");
    printf("\n");
    printf("  ctrl + c - press anytime to force quit a process\n");
    printf("  ctrl + d - press to send EOF\n");
    printf("  & - use comand & comand to execute two comands at the same time\n");
    printf("  backgroundTest - executes a process in background for a limited time. The process will notify when alive and dead\n");
    printf("  clear - clears screen\n");
    printf("  digitalTime - displays time in digital format. Press c to change colour and q to exit\n");
    printf("  echo TEXT - displays the TEXT in screen\n");
    printf("  exit - exits current shell and restarts it\n");
    printf("  help - prints a list containing all the available commands and its descriptions\n");
    printf("  invalid opcode - shows how the cero division exception is handled\n");
    printf("  memory - shows bock index in buddy allocation system with it's base adress and final address(exclusive)\n");
    printf("  messages - shows a pipe demostration\n");
    printf("  necesitoquemeapapachen - shows burro\n");
    printf("  philosophers - interactive dinning philosofers problem solution\n");
    printf("  pipesdemo - starts a simple program that creates two processes that say hi to each other using pipes");
    printf("  priority - shows scheduler priority\n");
    printf("  ps - shows information about the current processes in the system\n");
    printf("  sushi - interactive producer vs consumer problem solution with sushi\n");
    printf("  time - displays current time\n");
    printf("  zero division - shows how the cero division exception is handled\n");
    printf("  nice [PID] [1-10] - run a program with modified scheduling priority. \nNiceness values range from 1 (least favorable to the process)to 100 (most favorable to the process).\n");
    printf("  nice [PID] - prints the process niceness\n");

    printf(" \n This commands bellow can be used either alone, or to test pipes. You can try 'othercommand | thiscommand'\n");
    printf("  echoInput - Reads from stdin until it founds EOF\n");
    printf("  remark (param) - Reads from stdin until it founds EOF and catch the word you send in param in 2 brackets every time you type it\n");

    giveControlToShell();
}

void cleanScreen(){
    _syscall(_clearScreen);
}

void showTime() {
    printf("\n");
    printf(getTime());
    printf("\n");
    giveControlToShell();
}

void showDigitalHour(){
    Colour myColours[5] = {colour1, colour2, colour3, colour4, colour5};
    cleanScreen();
    printf("C - to change colour\n");
    printf("Q - to quit");
    char key = 0;
    int j = 0;
    while (1) {
        readfd(0,&key,1);
        switch (key) {
            case 'c': //hotkey
                _syscall(_beep);
                delay(5000);
                _syscall(_unbeep);
                j++;
                j = j % SIZE;
                break;
            case 'q':
                cleanScreen();
                giveControlToShell();
                return;
                break;
        }
        key = 0;
        drawTime(getTime(), myColours[j]);
    }
}

int div100(int n) {
    return 100 / n;
}


void divi(){
    div100(0);
    giveControlToShell();
}


void showOpcodeExc(){
    opcodeExc();
    giveControlToShell();
}

void delay(int i) {
    for (int j = 0; j < i; j++) {
        for (int k = 0; k < i; k++) {
        }
    }
}

void ps(){
    char* buffer = malloc(2000);
    printf("\n");
    printf("PID   STATE      MEMORYALLOCATED   PROCESSNAME");
    printf("\n");
    _syscall(_ps, buffer, 2000);
    printf("%s",buffer);

    printf("\n");
    free(buffer);
    giveControlToShell();
}

void memory(){
    char* buffer = malloc(3000);
    printf("\n");
    _syscall(_sprintMemory, buffer, 3000);
    printf("%s",buffer);
    free(buffer);
    giveControlToShell();
}

void showBurro() {
    printf("\n");
    printf("    ,--._ \n");
    printf("    `.   `.                      ,-. \n");
    printf("      `.`. `.                  ,'   ) \n");
    printf("          \`:  \               ,'    / \n");
    printf("          \`:  ),.         ,-' ,   / \n");
    printf("         ( :  |:::.    ,-' ,'   ,' \n");
    printf("          `.;: |::::  ,' ,:'  ,-' \n");
    printf("          ,^-. `,--.-/ ,'  _,' \n");
    printf("         (__        ^ ( _,' \n");
    printf("        __((o\   __   ,-' \n");
    printf("      ,',-.     ((o)  / \n");
    printf("    ,','   `.    `-- ( \n");
    printf("    |'      ,`        \ \n");
    printf("    |     ,:' `        | ");   printf("        This is burrOS\n");
    printf("   (  `--      :-.     | \n");
    printf("   `,.__       ,-,'   ; \n");
    printf("   (_/  `,__,-' /   ,' \n");
    printf("   |\`--|_/,' ,' _,' \n");
    printf("    \_^--^,',-' -'( \n");
    printf("    (_`--','       `-. \n");
    printf("     ;;;;'       ::::.`------. \n");
    printf("       ,::       `::::  `:.   `.\n");
    printf("      ,:::`       :::::   `::.  \\ \n");
    printf("     ;:::::       `::::     `::  \\ \n");
    printf("     |:::::        `::'      `:   ; \n");
    printf("     |:::::.        ;'        ;   |\n");
    printf("     |:::::;                   )  | \n");
    printf("     |::::::        ,   `::'   |  \\ \n");
    printf("     |::::::.       ;.   :'    ;   ::. \n");
    printf("     |::::,::        :.  :   ,;:   |:: \n");
    printf("     ;:::;`::     ,:::  |,- `:   |::, \n");
    printf("    /;::|    `--;""';'  |     :. (`; \n");
    printf("    \\   ;           ;   |     ::  `, \n");
    printf("     ;  |           |  ,:;     |  : \n");
    printf("     |  ;           |  |:;     |  | \n");
    printf("     |  |           |  |:      |  | \n");
    printf("     |  |           |  ;:      |  | \n");
    printf("    /___|          /____|     ,:__| \n");
    printf("   /    /         /    |     /    ) \n");
    printf("   `---'          '----'      `---' \n");

    giveControlToShell();
}

void stayAlive() {
    int i = 0;
    while (i < 20) {
        delay(15000);
        printf("still alive\n");
        i++;
    }
    printf("i am dead\n");
}

void maDemo(){
    int c;
    void* p;
    printf("\n");
    printf("MEMORY ALLOCATION DEMO: \n");
    printf("Press any key in order to visualize current process Memory Status\n");
    c = getChar();
    memory();

    printf("Press any key in order to allocate a block of size 128\n");
    c = getChar();
    p = malloc(128);
    printf("\nA block of 128 was allocated: \n");
    memory();

    printf("Press any key to free the block\n");
    c = getChar();
    free(p);

    printf("The block was freed\n");
    memory();

    giveControlToShell();
}

int getProcessPriority(int pid) {
    return _syscall(_getProcessPriority, pid);
}

void getProcessPriorityShell(char * sPid){
    int pid = atoi(sPid);
    if(pid <= 0){ //no hay proceso con pid 0. Significa que el usuario no ingreso un numero como argumento pid
        printf("\nThe process pid entered is invalid. Keep in mind it should be an integer\n");
    }
    int result = getProcessPriority(pid);
    if(result == 0){ //significa que el pid pasado es incorrecto
        printf("\nThere is no process identified with the pid entered\n");
    }
    else{
        printf("\nNiceness: %d", result);
    }
}

void niceShell(char * sPid, char * sNiceness){
    int result;
    int pid = atoi(sPid);
    int niceness = atoi(sNiceness);
    if(pid <= 0){ //no hay proceso con pid 0. Significa que el usuario no ingreso un numero como argumento pid
        printf("\nThe process pid entered is invalid. Keep in mind it should be an integer\n");
    }
    else if(niceness<= 0 || niceness > 100){ //la niceness va de 1-100. Significa que el usuario ingreso mal la niceness
        printf("\nThe niceness entered is invalid. It should be a number between 1-100\n");
    }
    else{
        result = nice(pid, niceness);
        if(result != 0){
            printf("There is no process identified with the pid entered\n");
        }
    }
    // printf("%d\n", pid);
    // printf("%d\n", niceness);
}

int getCurrentPid(){
    return _syscall(_getPid);
}

void process(int id) {
    int pid = getCurrentPid();
        while(1){
        delay(5000);
        delay(5000);
        delay(5000);
        delay(5000);
        printf("\nI am number %d. My priority is: %d\n", id, getProcessPriority(pid));
    }
}

void schedulerDemo() {
    printf("\nSCHEDULER PRIORITY DEMO\n");
    printf("\nPress 'i' to increase process 1 priority\n");
    printf("\nPress 'd' to decrease process 1 priority\n");
    printf("\n\nKeep in mind process' 2 priority stays in 1 throughout the demo\n");
    int processId = 1;
    int p1Pid = exec("process 1", process, processId++, 0);
    int p2Pid = exec("process 2", process, processId, 0);

    int running = 1;
    int priority = 1;
    int key = 0;
    while (running) {
        readfd(0,&key,1);
        switch (key) {
            case 'i':
                if(priority < 100) {
                    priority++;
                    nice(p1Pid, priority);
                }
                break;
            case 'd':
                if(priority > 1) {
                    priority--;
                    nice(p1Pid, priority);
                }
                break;
            case 'q':
                running = 0;
                kill(p1Pid, 0);
                kill(p2Pid, 0);
                break;
        }
        key = 0;
    }


    giveControlToShell();

}
