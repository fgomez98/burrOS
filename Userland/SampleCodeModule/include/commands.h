#ifndef commands_h
#define commands_h

#define CLOCK 2
#define WRITE 1
#define STDOUT 1
#define READ 0
#define STDIN 0
#define CLEAR 0
#define BEEP 3
#define SIZE 5
/*IMPORTANT: the imput of commands must be exactly as shown on "help" list, wuth no spaces before or after*/

/*Prints the available list of commands on sreen*/
void help();

/*Prints on the shell the time at which the command was entered, calls getTime() to get the hours, minutes and seconds using system calls*/
void showTime();

/*Launches a digital real-time clock on the shell. The colour can be changed by pressing c (theres a pre-defined selection of 5 colours). Press q to exit*/
void showDigitalHour();

/*Displays on screen the state of the registers when there is an OperationCodeEception, prgram doesnt work properly after this because of the nature of the exception*/
void showOpcodeExc();

/*Deletes everything present on the screen at the TimeModule*/
void cleanScreen();

/*Calls div100() with parameter 0, displays on screen the state of the registers when there is a ZeroDivisionEception, prgram doesnt work properly after this because of the nature of the exception*/
void divi();

/*Divides the integer 100 by the number n*/
int div100(int n);
void delay(int i);
void ls();
void ps();
void memory();
void showBurro();
void stayAlive();

void startProcessCommunication();
void maDemo();

#endif
