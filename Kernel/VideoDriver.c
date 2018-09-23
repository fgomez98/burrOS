//
//  VideoDriver.c
//
//
//  Created by Fermin Gomez on 5/23/18.
//
//
#include "VideoDriver.h"
#include "font.h"
#include "lib.h"
#define CHAR_WIDTH 8
#define CHAR_HEIGHT 16
static char buffer[64] = { '0' }; // esto es para imprimir hexa
#define Y_SPACE 2// espacio entre lineas
#define X_SPACE 1// espacio entre caracteres

typedef struct __attribute__((packed)) ModeInfoBlock {
        uint16_t ModeAttributes;
        uint8_t WinAAttributes;
        uint8_t WinBAttributes;
        uint16_t WinGranularity;
        uint16_t WinSize;
        uint16_t WinSegmentA;
        uint16_t WinSegmentB;
        uint32_t WinRealFctPtr;
        uint16_t pitch; // Bytes per ScanLine.

        uint16_t XResolution;
        uint16_t YResolution;
        uint8_t XcharSize;
        uint8_t YcharSize;
        uint8_t NumberOfPlanes;
        uint8_t BitsPerPixel;
        uint8_t NumberOfBanks;
        uint8_t MemoryModel;
        uint8_t BankSize;
        uint8_t NumberOfImagePages;
        uint8_t ReservedPage;

        uint8_t RedMaskSize;
        uint8_t RedMaskPosition;
        uint8_t GreenMaskSize;
        uint8_t GreenMaskPosition;
        uint8_t BlueMaskSize;
        uint8_t BlueMaskPosition;
        uint8_t ReservedMaskSize;
        uint8_t ReservedMaskPosition;
        uint8_t DirectColorAttributes;

        uint32_t PhysBasePtr;  // Your LFB (Linear Framebuffer) address.
        uint32_t OffScreenMemOffset;
        uint16_t OffScreenMemSize;
} Vesa;

Colour black = {0, 0, 0};
Colour white = {255, 255, 255};
static Vesa * video = (Vesa*)0x5C00;
static int XPOSITION = X_SPACE;
static int YPOSITION = Y_SPACE;
static int XPOSITION2 = X_SPACE;
static int YPOSITION2 = Y_SPACE;

void putPixel(int x, int y, Colour colour) {
	unsigned whereOnScreen = y*video->pitch + x*(video->BitsPerPixel/8);
	char * screen = (char *) (video->PhysBasePtr + whereOnScreen);
	*(screen) = colour.Red;
	*(screen + 1) = colour.Green;
	*(screen +2) = colour.Blue;
}

void putPixel2(int x, int y, int colour) {
    unsigned whereOnScreen = y*video->pitch + x*(video->BitsPerPixel/8);
    char * screen = (char *) (video->PhysBasePtr + whereOnScreen);
    *(screen) = colour & 255;
    *(screen + 1) = (colour >> 8) & 255;
    *(screen +2) = (colour >> 16) & 255;
}

Colour intToRGB(int i) {
    Colour colour;
    colour.Blue = (i & 255);
    colour.Green = ((i >> 8) & 255);
    colour.Red = ((i >> 16) & 255);
    return colour;
}

void printChar(char c, Colour colour) {
    if (c == 0) {
        return;
    }
    char font;
    char *  font_char = pixel_map(c);
    for (int y = 0; y < 16; y++) {
        font = font_char[y];
        for (int x = 0; x < 8; x++) {
            if (((font >> (8-x)) %2) != 0){
                putPixel(x + XPOSITION, y + YPOSITION, colour);
            } else {
                putPixel(x + XPOSITION, y + YPOSITION, black);
            }
        }
    }
}

// imprime un caracter en pantalla
void putChar(char c, Colour colour) {
    boundaryCorrector();
    if (c == '\n') {
        newLine();
        XPOSITION2 = XPOSITION;
        YPOSITION2 = YPOSITION;
        return;
    } else if (c == '\b') {
        deleteChar();
        return;
    }
    printChar(c, colour);
    XPOSITION += CHAR_WIDTH + X_SPACE;
    boundaryCorrector();
}

void deleteChar() {
    XPOSITION -= (CHAR_WIDTH + X_SPACE);
    if (XPOSITION2 > XPOSITION) {
        if (YPOSITION2 < YPOSITION) {
            XPOSITION += (CHAR_WIDTH + X_SPACE);
            return;
        }
    }
    boundaryCorrector();
    printChar(' ', black);
}

void putStrAux(char * str, Colour colour) {
    int i = strlen(str);
    if ((XPOSITION+(CHAR_WIDTH+X_SPACE)*i) > video->XResolution) {
        newLine();
    }
    char c;
    i = 0;
    while ( (c = str[i++])) {
        putChar(c, colour);
    }
}

// imprime un string en pantalla
void putStr(char * str, Colour colour) { // lee hasta el cero, si no lo tiene como hago para que corte en algun momento (excepcion)
    char buff[128] = {0};
    char c;
    int i = 0;
    int j = 0;
    while ((c=str[i++])) {
        buff[j++] = c;
        if (c == ' ') {
            buff[j] = 0;
            putStrAux(buff, colour);
            memSet(buff, 0, j);
            j = 0;
        } else if (c == '\n') {
            newLine();
        }
    }
    if (c == 0) {
        putStrAux(buff, colour);
    }
}

void newLine() {
    YPOSITION += 16 + Y_SPACE;
    XPOSITION = X_SPACE;
    boundaryCorrector();
}

void boundaryCorrector() {
    if ((video->XResolution - X_SPACE) <= XPOSITION || ((video->XResolution - XPOSITION - X_SPACE) < (8+X_SPACE))) {
        newLine();
    } else if ((video->YResolution -Y_SPACE - CHAR_HEIGHT) <= YPOSITION) {
        YPOSITION -= (CHAR_HEIGHT + Y_SPACE);
        moveScreenUp();
    } else if (YPOSITION < Y_SPACE) {
        YPOSITION = Y_SPACE;
    } else if (XPOSITION < X_SPACE) {
        if (YPOSITION > (Y_SPACE + CHAR_HEIGHT)) {
            YPOSITION -= (Y_SPACE + CHAR_HEIGHT);
            int aux = video->XResolution / (X_SPACE + CHAR_WIDTH); // cant que entra
            XPOSITION = (X_SPACE + CHAR_WIDTH)*aux+X_SPACE;
        } else {
            XPOSITION = X_SPACE;
        }
    }
}

//corre las lineas hacia arriba excluyendo la ultima
void moveScreenUp() {
    unsigned whereOnScreen = (CHAR_HEIGHT+(2*Y_SPACE))*(video->pitch) + X_SPACE*(video->BitsPerPixel/8);
    char * source = (char *) (video->PhysBasePtr + whereOnScreen);
    unsigned whereOnScreen2 = (Y_SPACE)*(video->pitch) + X_SPACE*(video->BitsPerPixel/8);
    char * dest = (char *) (video->PhysBasePtr + whereOnScreen2);
    int size = (video->YResolution)*(video->XResolution)*3;
    memCpy(dest, source, size);
    for (int y = YPOSITION ; y < video->YResolution; y ++) { //Y_SPACE
        for (int x = X_SPACE; x < video->XResolution-X_SPACE; x++) {
            putPixel(x, y , black);
        }
    }
}

// modo shell
//al inicio esta en modo screen, inserta en la pantalla en la parte superior
void modeScreen() {
    clearScreen();
    XPOSITION = X_SPACE;
    YPOSITION = Y_SPACE;
}

// borra la pantalla a partir de la poscion (x, y) dada
void clear(int i, int j) {
    for (int y = j; y < video->YResolution; y++) {
        for (int x = i; x < video->XResolution; x++) {
            putPixel(x, y, black);
        }
    }
}

// borra la linea de comandos en pantalla
void clearComand() {
    clear(0, (video->YResolution-CHAR_HEIGHT-Y_SPACE));
}

// borra la pantalla entera
void clearScreen() {
    clear(0, 0);
}

// longitud de palabra
int strlen(const char * str) {
    int i = 0;
    while(*(str+i)) {
        i++;
    }
    return i;
}

//pasar de uint64_t a hexa y de ahi imprmir en pantalla
void putHexa( uint64_t value, Colour colour){
  	printBase(value, 16, colour);
}

void printBase(uint64_t value, uint64_t base, Colour colour)
{
    uintToBase(value, buffer, base);
    putStr(buffer, colour);
}
