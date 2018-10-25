#ifndef String_h
#define String_h

/*Recieves an integer number on base 10 and loads it on a char array, returns the array length*/
int itos(char*str, int num);

/*Returns the amount of digits of the number passed by parameter*/
int numlength(int num);
char* strcpy(char* destination, const char* source);
/*Returns the length of the string passed by parameter*/
// int strlen(const char* str);

/*Compares the two strings character to character, returns 0 if they are equal, -1 if the first string has a lower char value or 1 if the first string has a higher char value*/
// int strcmp(char string1[], char string2[] );

#endif
