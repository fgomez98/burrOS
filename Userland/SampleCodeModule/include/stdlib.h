#ifndef lib_h
#define lib_h

/*Recieves a char array containing anumber and returns said number as an integer(base 10)*/
int atoi(char * str);

/*Returns 1 if c is a number, else it returns 0*/
int isNum(char c);

/*Returns 1 if c is a letter(Uppercase or Lowercase), else it returns 0*/
int isAlpha(char c);

/*Returns 1 if c is a space, else it returns 0*/
int isSpace(char c);

/*Returns 1 if c is an Uppercase letter, else it returns 0*/
int isSuper(char c);

#endif
