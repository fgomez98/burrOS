#include "KeyboardDriver.h"
#include "VideoDriver.h"
#include "ScanCodes.h"
#include "Keyboard.h"
#define FALSE 0
#define TRUE 1
#define SIZE 128

static char buffer[SIZE] = {0};
static int putIndex = 0;
static int getIndex = 0;
static int size = 0;
static int SHIFT_ON = FALSE;
static int CAPS_ON = FALSE;
static int CNTRL_ON = FALSE;

// Code based from OSDEV

void Keyboard_Handler() { // once a key is pressed, it calls an interrupton which the IDT will call this function
    unsigned int scan = getKey();
    char input;
    if (scan & 0x80) { //if first bit is 1 then the key was released
        if (scan == 0xAA || scan == 0xB6) { // shift
            SHIFT_ON = FALSE;
        }
    } else { // a key was pressed,is another one was already pressed, and it remains being pressed, it will send multiple iterruption,if it is a special key set on TRUE, pressing another key will access to its special map
        switch (scan) {
            case RSHIFT: // shift
                SHIFT_ON = TRUE;
                break;
            case LSHIFT: // shift
                SHIFT_ON = TRUE;
                break;
            case CAPS:  // caps
                CAPS_ON = !CAPS_ON;
                break;
        }
        if (SHIFT_ON) {
            input = getAsciiShiftCode(scan);
            addToBuffer(input);
        } else if (CAPS_ON) {
            input = getAsciiCode(scan);
            if (input>='a' && input<='z') {
                input -= 32;
            }
            addToBuffer(input);
        } else {
            input = getAsciiCode(scan);
            addToBuffer(input);
        }
    }
}

char getKeyInput() {
    if (size <= 0) {
        return 0;
    }
    char c = buffer[getIndex];
    buffer[getIndex] = 0;
    getIndex = (getIndex+1)%SIZE;
    size--;
    return c;
}

void addToBuffer(char input) {
    buffer[putIndex] = input;
    putIndex = (putIndex+1)%SIZE;
    size++;
}
