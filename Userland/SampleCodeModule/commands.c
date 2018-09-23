#include "commands.h"
#include "stdio.h"
#include <stdint.h>
#include "VideoModule.h"
#include "TimeModule.h"
#include "systemCall.h"
#define CLOCK 2
#define WRITE 1
#define STDOUT 1
#define READ 0
#define STDIN 0
#define CLEAR 0
#define BEEP 3
#define SIZE 5

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
  printf("help - prints a list containing all the available commands and its descriptions\n");
  printf("digitalTime - displays time in digital format. Press c to change colour and q to exit\n");
  printf("time - displays current time\n");
  printf("clear - clears screen\n");
  printf("zero division - shows how the cero division exception is handled\n");
  printf("invalid opcode - shows how the cero division exception is handled\n");
  printf("ps - shows information about the current processes in the system\n");
  printf("exit - exits from shell\n");
}

void cleanScreen(){
  systemCall(WRITE, CLEAR, 0,0, 0, 0);
}

void showTime() {
    printf("\n");
    printf(getTime());
    printf("\n");
}

void showDigitalHour(){
    Colour myColours[5] = {colour1, colour2, colour3, colour4, colour5};
    cleanScreen();
    char key;
    int j = 0;
    while (1) {
        systemCall(READ, STDIN, &key,1, 1,0);
        switch (key) {
            case 'c': //hotkey
                systemCall(WRITE, BEEP, 0,0,0,0);
                delay(5000);
                systemCall(WRITE, BEEP, 1,0,0,0);
                j++;
                j = j % SIZE;
                break;
            case 'q':
                cleanScreen();
                return;
                break;
        }
        drawTime(getTime(), myColours[j]);
    }
}

int div100(int n) {
  return 100 / n;
}


void divi(){
  div100(0);
}


void showOpcodeExc(){
  opcodeExc();
}

void delay(int i) {
    for (int j = 0; j < i; j++) {
        for (int k = 0; k < i; k++) {
        }
    }
}

void ps(){
  char* buffer = malloc(1024);
  printf("\n");
  printf("pid   state");
  printf("\n");
  systemCall(8, buffer, 1024, 0, 0, 0);
  printf(buffer);
  free(buffer);
}
