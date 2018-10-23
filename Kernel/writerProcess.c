#include <BuddyAllocationSystem.h>
#include <readwrite.h>
#include <videoDriver.h>


Colour writerColor = {255, 255, 255};

void writeToScreen() {
    while(1) {
        int i = 0;
        char * buffer = mallocMemory(BUFFERSIZE + 1);
        i = read(STDOUT_FILENO, buffer, BUFFERSIZE);
        buffer[i] = '\0';
        putStr(buffer, writerColor);
    }
}