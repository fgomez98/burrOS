#include "VideoModule.h"
#include "syscall.h"
#define XRESOLUTION 1024
#define YRESOLUTION 768
#define STDOUT 1
#define WRITE 1

void drawPixel(int x, int y, Colour colour) {
    int rgb = colour.Red;
    rgb = (rgb << 8) + colour.Green;
    rgb = (rgb << 8) + colour.Blue;
    _syscall(_drawPixel, x, y, rgb);

}

void drawDigitalColon(Colour colour, int x, int y) {
    drawDigitalClockExp(colour, colon_map(), x, y);
}

void drawDigitalNumber(Colour colour, int number, int x, int y) {
    drawDigitalClockExp(colour, clock_map(number), x, y);
}
void drawDigitalClockExp(Colour colour, unsigned char * fontExp, int xPosition, int yPosition) {
    char font;
    int xpos = xPosition;
    for (int y = 0; y < 36; y++) {
        for (int j = 0; j < 8; j++) {
            font = fontExp[(y*8)+j];
            for (int x = 0; x < 8; x++) {
                if (((font >> (8-x)) %2) != 0){
                    drawPixel(x + xPosition, y + yPosition, colour);
                } else {
                    Colour black = {0,0,0};
                    drawPixel(x + xPosition, y + yPosition, black);
                }
            }
            xPosition += 8;
        }
        xPosition = xpos;
    }
}

void drawTime(char * time, Colour colour) {
    int xPosition = (XRESOLUTION/2) - 281;
    int yPosition = (YRESOLUTION/2) - 18;
    char c;
    int i = 0;
    while ((c=time[i++])) {
        if (c == ':') {
            drawDigitalColon(colour, xPosition, yPosition);
        } else {
            drawDigitalNumber(colour, (c - '0'), xPosition, yPosition);
        }
        xPosition += (64 + 10); // 10 es la distancia entre digitos
    }
}

void drawSushiStateAux(Colour colour, int sushiManAmount, int monsterAmount) {
    int xPosition = (XRESOLUTION/2) - (195+120);
    int yPosition = (YRESOLUTION/2) + 150;
    drawDigitalClockExp(colour, clock_map(5), xPosition, yPosition);
    xPosition += (64);
    drawDigitalColon(colour, xPosition, yPosition);
    xPosition += (64);
    drawDigitalClockExp(colour, clock_map(sushiManAmount), xPosition, yPosition);
    xPosition += (64 + 120);
    drawDigitalClockExp(colour, letterM_map(), xPosition, yPosition);
    xPosition += (64);
    drawDigitalColon(colour, xPosition, yPosition);
    xPosition += (64);
    drawDigitalClockExp(colour, clock_map(monsterAmount), xPosition, yPosition);
}

void drawSushiState(int sushiManAmount, int monsterAmount) {
    Colour white1 = {255, 255, 255};
    drawSushiStateAux(white1, sushiManAmount, monsterAmount);
}

void clearSushiState(int sushiManAmount, int monsterAmount) {
    Colour black = {0,0,0};
    drawSushiStateAux(black, sushiManAmount, monsterAmount);
    
}
