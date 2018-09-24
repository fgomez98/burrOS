//
//  String.h
//
//
//  Created by Fermin Gomez on 5/22/18.
//
//

#ifndef String_h
#define String_h

#include <stdio.h>

/*copies from source to destination until a \0 is read*/
int strlenght(char * s);
char * strconcat(char * s1, char * s2, char * dest);
char* strcpy(char* destination, const char* source);
int strcmp(char string1[], char string2[] );
void intToString(char* str, int num);
void reverseCharArray(char* str, int length);
int numLength(int num);


#endif /* String_h */
