#ifndef LIB_H
#define LIB_H

#include <stdint.h>

void * memset(void * destination, int32_t character, uint64_t length);
void * memcpy(void * destination, const void * source, uint64_t length);
char *cpuVendor(char *result);
uint64_t uintToBase(uint64_t value, char * buffer, uint64_t base);
void memCpy(void *dest, void *src, int n);
void memSet(void * src, int c, int n);
int abs(int num);
#endif