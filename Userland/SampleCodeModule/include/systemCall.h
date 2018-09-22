
#ifndef systemCall_h
#define systemCall_h
#include <stdint.h>

/*Allows the connection between Userland and Kernel, it sengs by stack 6 arguments which dictate the behaviour and which functions to call, to produce a certain input or output.
The arguments can take the following values:
arg1: can be WRITE (1) or READ (2), determines if the systcall will be to put or retrieve from screen.
arg2: STDIN (0), STDOUT (1), CLOCK (2) & BEEP (3). It defines where it will access, Screen, Clock or Speaker.
arg3: Used to define which type of outpt will be selected when arg2 is STDOUT (putStr, putChar, deleteChar, etc), the type of beep when accesing clock,
 or works as a pointer to fetch a character when called in RREAD mode.
arg4, arg5 &a arg6:multi use arguments, used to send values to kernel containing, for exampe, coodinates for printing a pixel or a given colour, or retrieving hours, minutes or seconds with getTime.
 */
unsigned int systemCall(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif
