//
//  String.c
//  
//
//  Created by Fermin Gomez on 5/22/18.
//
//

#include "String.h"

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
