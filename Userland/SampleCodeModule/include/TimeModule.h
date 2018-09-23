#ifndef TimeModule_h
#define TimeModule_h

#include <stdint.h>

/*Creates a new buffer and loads it with the current hour by using system calls to get the hours, minutes and seconds. Resulting array finalizes with \0*/
char * getTime();

#endif
