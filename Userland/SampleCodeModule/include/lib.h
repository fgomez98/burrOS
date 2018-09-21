#ifndef lib_h
#define lib_h
#include <stdint.h>

#define MAXLENGTH 100

/*recieves a uint64_t value, a base and and an empy char array, and converts the number to that base and leaves it in the array as chars, similar to how atoi works, but the other way*/
uint64_t uintToBase(uint64_t value, char * buffer, uint64_t base);

#endif
