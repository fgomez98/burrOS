//
//  String.c
//
//
//  Created by Fermin Gomez on 5/22/18.
//
//

#include "String.h"
int strcpy2(char* str1, const char* str2, int length);

char* strcpy(char* destination, const char* source) {
    if (destination == NULL) {
        return NULL;
    }
    char *ptr = destination;
    while (*source) {
        *destination = *source;
        destination++;
        source++;
    }
    *destination = 0;
    return ptr;
}

int strcmp(char string1[], char string2[] ){
    for (int i = 0; ; i++)
    {
        if (string1[i] != string2[i])
        {
            return string1[i] < string2[i] ? -1 : 1;
        }
        
        if (string1[i] == '\0')
        {
            return 0;
        }
    }
}
