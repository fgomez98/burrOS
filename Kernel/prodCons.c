#include "mutex.h"
#include "VideoDriver.h"
#include "semaphore.h"
#include "prodCons.h"
#define MAXMONSTERS 10
#define MAXSUSHIMEN 10
#define BUFFER_LIMIT 128
#define BUFFMUTEX "prodConsBufferMutex"
#define ITEMS "prodConsItemsSem"
Colour colour8 = {180, 100, 255};
char buff8[8];
//static char * tunaRoll = " ,;'@@';,\n|',_@@_,'|\n|        |\n '.____.' ";
//static char * sake  = "  ,iiiiiiiiii,\n,iiiiiiiiiiiiii,\niii'        'ii'\n  '.________.' ";
static mutex * buffMutex;
static sem * items;
static char * buffer;
static int bufferIndex;
static int running = 1;
static char monsterCount;
static char sushiMenCount;


void initProdCons() {
    monsterCount = 1;
    sushiMenCount = 1;
    buffer = mallocMemory(500);
    bufferIndex = 0;
    items = getSem(ITEMS);
    buffMutex = getMutex(BUFFMUTEX);
    clearScreen();
    putStr("...Welcome to: agodio/itba-so:1.0 SuhiBar...", colour8);
     putChar('\n', colour8);
    putStr("Initially there is one shushiMan and one eating monster", colour8);
     putChar('\n', colour8);
    putStr("Press q to quit", colour8);
     putChar('\n', colour8);
    putStr("Press s to add a sushiman", colour8);
     putChar('\n', colour8);
    putStr("Press m to add an eating monster", colour8);
     putChar('\n', colour8);
    putStr("Press i to start", colour8);
    putChar('\n', colour8);
    tProcess * proc;
    char initialized = 0;
    while (running) {
        char key = getKeyInput();
        switch (key) {
            case 's':
                if ((sushiMenCount < MAXSUSHIMEN) && initialized) {
                    proc = createProcess("sushiMan", sushiMan, 0, 0, NULL);
                    addProcess(proc);
                }
                break;
            case 'm':
                if ((monsterCount < MAXMONSTERS) && initialized) {
                    proc = createProcess("monster",  monster, 0, 0, NULL);
                    addProcess(proc);
                }
                break;
            case 'q':
                running = 0;
                break;
            case 'i':
                initialized = 1;
                proc = createProcess("sushiMan", sushiMan, 0, 0, NULL);
                addProcess(proc);
                proc = createProcess("monster",  monster, 0, 0, NULL);
                addProcess(proc);
                break;
        }
    }
    putStr("Game Over", colour8);
    endProcess(getRunningPid());
}


int myRand() {
    static int next = 3251;
    next = ((next * next) / 100 ) % 10000;
    return next ;
}

int myRandInRange (int min, int max) {
    return myRand() % (max+1-min) + min ;
}

void sushiMan() {
    while(running) {
        char * sushi = makeSushi();
        
        adquire(buffMutex);
        
        addToBuffer_(sushi);
        printBuffer();
        
        //post(items);
        release(buffMutex);
    }
    endProcess(getRunningPid());
}

void addToBuffer_(char * sushi) {
    int i = 0;
    while ((bufferIndex < (BUFFER_LIMIT-1)) &&  (*(sushi+i) != '\0')) {
        *(buffer+bufferIndex) = *(sushi+i);
        i++;
        bufferIndex++;
    }
    *(buffer+bufferIndex) = '\0';
}

void printBuffer() {
    clearScreen();
    putStr(buffer, colour8);
}

char * makeSushi() {
    int i;
    int suhiAmount = myRandInRange(1, 10);
    char * sushi = mallocMemory((suhiAmount*2)+1);
    for (i = 0; i < suhiAmount; i++) {
        *(sushi+i) = 'o';
        i++;
        *(sushi+i) = ' ';
    }
     *(sushi+i) = '\0';
    return sushi;
}

void monster() {
    while(running) {
        //wait(items);
        adquire(buffMutex);
        
        eatFromBuffer();
        //printBuffer();

        
        release(buffMutex);
        
    }
    endProcess(getRunningPid());
}
           
void eatFromBuffer() {
    int suhiAmount = myRandInRange(1, 10);
    while(suhiAmount && bufferIndex) {
        *(buffer+bufferIndex) = '\0';
        deleteChar();
        bufferIndex--;
        *(buffer+bufferIndex) = '\0';
        deleteChar();
        bufferIndex--;
        suhiAmount--;
    }
}

