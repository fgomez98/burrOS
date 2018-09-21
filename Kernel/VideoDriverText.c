#include <naiveConsole.h>

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base);

static char buffer[64] = { '0' };
static uint8_t * const video = (uint8_t*)0xB8000;
static uint8_t * currentVideo = (uint8_t*)0xB8000;
static const uint32_t width = 80;
static const uint32_t height = 25;

static uint32_t _width = 80;
static const uint32_t COLOUR = 255;
static uint32_t _height = 25;

void ncPrint(const char * string)
{
	int i;

	for (i = 0; string[i] != 0; i++)
		ncPrintChar(string[i]);
}

void ncPrintChar(char character)
{
	*currentVideo = character;
	currentVideo += 2;
}

void ncNewline()
{
	do
	{
		ncPrintChar(' ');
	}
	while((uint64_t)(currentVideo - video) % (width * 2) != 0);
}

void ncPrintDec(uint64_t value)
{
	ncPrintBase(value, 10);
}

void ncPrintHex(uint64_t value)
{
	ncPrintBase(value, 16);
}

void ncPrintBin(uint64_t value)
{
	ncPrintBase(value, 2);
}

void ncPrintBase(uint64_t value, uint32_t base)
{
    uintToBase(value, buffer, base);
    ncPrint(buffer);
}

void ncClear()
{
	int i;

	for (i = 0; i < height * width; i++)
		video[i * 2] = ' ';
	currentVideo = video;
}

static uint32_t uintToBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}
/*
static void showTime(int colour, char * time){
    ncClear();
    for(int i = 0, j = 0; i < 8; i++){
        currentVideo + 2*64 + i = time[j++];
        currentVideo + 2*64 + i + 1 = colour;
    }
}
 */


//void draw_string(screen, where, char* input) {
//    while(*input) {
//        draw_char(screen,where,font_data[input]);
//        where += CHAR_WIDTH;
//        input++;
//    }
//}
//
//void draw_char(screen, where, font_char*) {
//    for (l = 0; l < CHAR_WIDTH; l++) {
//        for (i = 8; i > 0; i--) {
//            j++;
//            if ((font_char[l] & (1 << i))) {
//                c = COLOUR;
//                putpixel(currentVideo,j, h, c);
//            }
//        }
//        h++;
//        j = x;
//    }
//}
//
//static void putpixel(unsigned char* screen, int x,int y, int color) {
//    unsigned where = x*3 + y*2400;
//    screen[where] = color & 255;              // BLUE
//    screen[where + 1] = (color >> 8) & 255;   // GREEN
//    screen[where + 2] = (color >> 16) & 255;  // RED
//}

