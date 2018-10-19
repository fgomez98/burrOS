#ifndef ClockFont_H
#define ClockFont_H

/*CONTAINS THE FONTS FOR PRINTING THE DIGITAL CLOCK BY USING PUTPIXEL, IT USES X TO MARK PIXELS TO PRINT AND _ TO MARK SPACES TO BE IGNORED(BACKGROUND)*/

/*Given an integer number, it returns a font containing the positions on the grid to be able to print said number*/
unsigned char * clock_map(int number);

/*returns the font to be able to print ':'*/
unsigned char * colon_map();

unsigned char * letterM_map();

#define BIG_NUMBER_WIDTH 64
#define BIG_NUMBER_HEIGHT 36

#define _ 0x00
#define X 0xFF

#endif
