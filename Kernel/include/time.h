#ifndef _TIME_H_
#define _TIME_H_

/*Controls the internal clock of the system, using ticks to control time elapsed*/
void timer_handler();
int ticks_elapsed();
int seconds_elapsed();
char * getTime();
void showTime();
void updateCoulourAndBeep();
void displayTime();

#endif
