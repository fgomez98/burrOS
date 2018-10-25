#ifndef shell_h
#define shell_h

typedef void (*functionType)();
/*Loads onto screen a new program shell, in which the user interacts by using commands(see commands.h for more) */
void initializeShell();
void processCommand(char * command, char * arg1, char * opMode, char * arg2, char * param);
functionType getFunction(char * arg, int argc, int * flag);

#endif
