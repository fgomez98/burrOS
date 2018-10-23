//
//  VideoDriver.h
//
//
//  Created by Fermin Gomez on 5/23/18.
//
//
#include <stdint.h>
#ifndef VideoDriver_h
#define VideoDriver_h

	typedef struct {
		uint8_t Red;
		uint8_t Green;
		uint8_t Blue;
	} Colour;

    Colour intToRGB(int i);
    void moveScreenUp();
    void putPixel2(int x, int y, int colour);
	void putPixel(int x, int y, Colour color);
    void putStr(char * str, Colour colour);
    void putChar(char c, Colour colour);
	int RGBColourToInt(Colour colour);
    void putDigitalColon(Colour colour);
    void putDigitalNumber(Colour colour, int number);
    void modeScreen();
    void modeDigitalClock();
    void clearScreen();
    void clearComand();
    void newLine();
    void printBase(uint64_t value, uint64_t base, Colour colour);
	void putHexa( uint64_t number, Colour colour);
    void putTime(char * time, Colour colour);
    void deleteChar();
    void newComand();
    void putStrWithSize(char * str, Colour colour, int amount);

#endif /* VideoDriver_h */
