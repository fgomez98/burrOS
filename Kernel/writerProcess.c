#include <BuddyAllocationSystem.h>
#include <readwrite.h>
#include <videoDriver.h>


Colour writerColor = {255, 0, 0};

void writeToScreen() {
    while(1) {
        int i = 0;
        char * buffer = mallocMemory(BUFFERSIZE + 1);
        i = read(STDOUT_FILENO, buffer, BUFFERSIZE);
        putStrWithSize(buffer, writerColor, i);
    }
}