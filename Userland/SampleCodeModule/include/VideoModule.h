#ifndef VideoModule_h
#define VideoModule_h

#include <stdint.h>
#include "ClockFont.h"

typedef struct {
    uint8_t Red;
    uint8_t Green;
    uint8_t Blue;
} Colour;

/*Given an array containing a certain time and a colour(using the colour structure), it shows on screen a digital real time clock,
 by using calls to constanly refresh and print the time, until the quit hotckey is pressed. It uses the drawPixel() function and a list of pre set fonts for the numbers, containing ordered pixels*/
void drawTime(char * time, Colour colour);

/*Given a position on the grid that makes the shell, and a colour(using the colour structure), it prints a pixel of that colour in said position */
void drawPixel(int x, int y, Colour colour);

#endif
