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
char* strcpy(char* destination, const char* source);
void intToString(char* str, int num);
void reverseCharArray(char* str, int length);
int numLength(int num);


#endif /* String_h */
