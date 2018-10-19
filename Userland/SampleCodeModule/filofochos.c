#include "filofochos.h"
#define MIDX XRESOLUTION/2
#define MIDY YRESOLUTION/2

static int running;
static int filofochosAmount;

static Colour white = {255, 255, 255};
static Colour black = {0,0,0};
static Colour filofochoColours[] = {
    {100, 100, 255},
    {180, 40, 18},
    {100, 1000, 255},
    {50, 50, 280},
    {180, 180, 180}
};

void initFilofochos() {
    running = 1;
    filofochosAmount = 2;
    char initialized = 0;
    cleanScreen();
    printf("Welcome to Dinning Philosofers\n");
    printf("Initially there are 2 philosofers\n");
    printf("Press q to quit\n");
    printf("Press z to add a philosofer to the table (max 5)\n");
    printf("Press x to kill a philosofer\n");
    printf("Press i to start\n");
    while (!initialized) {
        char key;
        _syscall(_read, &key);
        switch (key) {
            case 'q':
                running = 0;
                initialized = 1;
                break;
            case 'i':
                initialized = 1;
                break;
        }

    }
    while (running) {
        drawTable();
        char key;
        _syscall(_read, &key);
        switch (key) {
            case 'z':
                if (filofochosAmount < 5) {
                    clearTable();
                    filofochosAmount++;
                }
                break;
            case 'x':
                if (filofochosAmount > 2) {
                    clearTable();
                    filofochosAmount--;
                }
                break;
            case 'q':
                running = 0;
                break;
            }
    }
    clearTable();
}

void drawFilofocho(Colour colour, int x, int y) {
    DrawFilledCircle(x, y, 35, colour);
}

void drawFilofochos() {
    switch (filofochosAmount) {
        case 2:
            drawFilofocho(filofochoColours[0], MIDX, MIDY + 215);
            drawFilofocho(filofochoColours[1], MIDX, MIDY - 215);
            break;
        case 3:
            drawFilofocho(filofochoColours[0], MIDX - 150, MIDY + 150);
            drawFilofocho(filofochoColours[1], MIDX + 150, MIDY + 150);
            drawFilofocho(filofochoColours[2], MIDX, MIDY - 215);
            break;
        case 4:
            drawFilofocho(filofochoColours[0], MIDX - 150, MIDY + 150);
            drawFilofocho(filofochoColours[1], MIDX + 150, MIDY + 150);
            drawFilofocho(filofochoColours[2], MIDX + 150, MIDY - 150);
            drawFilofocho(filofochoColours[3], MIDX - 150, MIDY - 150);
            break;
        case 5:
            drawFilofocho(filofochoColours[0], MIDX - 160, MIDY + 170);
            drawFilofocho(filofochoColours[1], MIDX + 160, MIDY + 170);
            drawFilofocho(filofochoColours[2], MIDX + 180, MIDY - 80);
            drawFilofocho(filofochoColours[3], MIDX - 180, MIDY - 80);
            drawFilofocho(filofochoColours[4], MIDX, MIDY - 215);
            break;
    }
}

void clearFilofochos() {
    switch (filofochosAmount) {
        case 2:
            drawFilofocho(black, MIDX, MIDY + 215);
            drawFilofocho(black, MIDX, MIDY - 215);
            break;
        case 3:
            drawFilofocho(black, MIDX - 150, MIDY + 150);
            drawFilofocho(black, MIDX + 150, MIDY + 150);
            drawFilofocho(black, MIDX, MIDY -215);
            break;
        case 4:
            drawFilofocho(black, MIDX - 150, MIDY + 150);
            drawFilofocho(black, MIDX + 150, MIDY + 150);
            drawFilofocho(black, MIDX - 150, MIDY - 150);
            drawFilofocho(black, MIDX + 150, MIDY - 150);
            break;
        case 5:
            drawFilofocho(black, MIDX, MIDY - 215);
            drawFilofocho(black, MIDX - 160, MIDY + 170);
            drawFilofocho(black, MIDX + 160, MIDY + 170);
            drawFilofocho(black, MIDX - 180, MIDY - 80);
            drawFilofocho(black, MIDX + 180, MIDY - 80);
            break;
    }
}

void drawTable() {
    drawCircle(MIDX, MIDY, 300, white);
    drawFilofochos();
}

void clearTable() {
    drawCircle(MIDX, MIDY, 300, black);
    clearFilofochos();
}
