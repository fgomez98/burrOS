#include "VideoDriver.h"
#include "Keyboard.h"
#include <stdint.h>
#include "math.h"
#include "Beep.h"
#include "TimeDriver.h"
#include "BuddyAllocationSystem.h"
#include "scheduler.h"
#include "String.h"
#include "mutex.h"
#include "semaphore.h"

#define WRITE 1
#define READ 0
#define STDOUT 1
#define STDIN 0
#define CLEAR 0
#define CLOCK 2
#define BEEP 3
static char buff[8];

/*For more information visit systemCall.h*/

void write(uint64_t arg2, uint64_t arg3, uint64_t arg4,  uint64_t arg5,  uint64_t arg6);
void read(uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5 , uint64_t arg6);
void sysKill(int pid, int message);
void syscall_handler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

/*First function called, it defines if the systemCall will be READ or WRITE, using arg1 decide*/
void syscall_handler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6) {
	void** pp;
	void* p;
	uint64_t* ui;
	tProcess* process;
	mutex* m ;
    sem* s;
	Colour colour;
  colour.Red = 255;
  colour.Green = 255;
  colour.Blue = 255;
	switch(arg1) {
    case READ:
			read(arg2, arg3, arg4, arg5, arg6);
			 break;
		case WRITE:
      write(arg2, arg3, arg4, arg5, arg6);
	 	break;
		case 3:
			pp = (void **)arg3;
		  *pp =  mallocMemoryInProcess(arg2, getRunningProcess());
		break;
		case 4:
		 		freeMemoryInProcess(arg2, getRunningProcess());
		break;
		case 5:
		 	 process = createProcess("default", arg2, 0, arg3, NULL);//despues ver bien lo del primer parametro q es el process name
    																													 //volar tamb lo de parent pid
			addProcess(process);
		break;
		case 6:
			sysKill(arg2, arg3);
		break;
		case 7:
			endProcess(getRunningProcess()->pid);
		break;
		case 8:
			sprintProcesses(arg2, arg3);
		break;
		case 9:
			sprintMemory(arg2, arg3);
		break;
		case 10:
			ui = (uint64_t*)arg3;
			*ui = getMutex(arg2);
		break;
		case 11:
			m = (mutex*) arg2;
			destroyMutex(m);
		break;
		case 12:
			m = (mutex*) arg2;
			adquire(m);
		break;
		case 13:
			m = (mutex*) arg2;
			release(m);
		break;
        case 14:
            ui = (uint64_t*)arg3;
            *ui = getSem(arg2);
            break;
        case 15:
            s = (sem*) arg2;
            destroySem(s);
            break;
        case 16:
            s = (sem*) arg2;
            wait(s);
            break;
        case 17:
            s = (sem*) arg2;
            post(s);
            break;
	}
}

/*Prints on screen a character, string or pixel in colour white(structure defined below), depending on the parameters recieved by arguments*/
void write(uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6){
  Colour colour;
  colour.Red = 255;
  colour.Green = 255;
  colour.Blue = 255;

	/*Funnels the second argument to call the function belonging to said argument, depending on what is sent from Userland*/
  switch(arg2){
    case CLEAR: //deletes everything from screen
			modeScreen();
    	break;
    case STDOUT:
        switch(arg3){
            case 0:
                putStr(arg4, colour); //puts the string recieved by a char pointer on screen, in white
                break;
            case 1:
                putChar(arg4, colour);//puts the char recieved by a char pointer on screen, in white
                break;
            case 3:
						deleteChar(); //deletes the next available character
                break;
            case 4:
                newLine(); //goes to the next line on screen
                break;
            case 5:
                ; // Nothing
                Colour colour2 = intToRGB(arg6); // changes colour to GRB format
                putPixel(arg4, arg5, colour2); //puts pixel on screen in arg4 (x) and arg5 (y) positions, in selected colour
                break;
            case 6:
                ; //Nothing
                int * p = (int *) arg5;
                switch(arg4){ //based on the int in arg4, it returns the hour, minute, and second the command was callede in a pointer in arg 5
                    case 0:
                        *(p) = getHour();
                        break;
                    case 1:
                        *(p) = getMin();
                        break;
                    case 2:
                        *(p) = getSec();
                        break;
                }
                break;
        }
        break;

    case BEEP: //calls an assembly code to execute a sound by speaker
        switch (arg3) {
            case 0:
                beep();
                break;
            case 1:
                unBeep();
                break;
        }
        break;
  }
}

/*Used to get the next available character from the keyboard buffer and returns it in arg3*/
void read(uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5 , uint64_t arg6){
	char * p =(char *) arg3;
	*(p) =  getKeyInput();
}


void sysKill(int pid, int message){
	switch(message){
		case 0:
			endProcess(pid);
		break;
		case 1:
			blockProcess(pid);
		break;
		case 2:
			unblockProcess(pid);
		break;
	}
}
