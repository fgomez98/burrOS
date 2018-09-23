#include <time.h>
#include "TimeDriver.h"
#include "String.h"
#include "lib.h"
#include "VideoDriver.h"
#include "Keyboard.h"
#include "Beep.h"
#define COLOUR_SIZE 6

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
