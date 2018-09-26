#include "prodCons.h"
#define MAXMONSTERS 5
#define MAXSUSHIMEN 5
#define BUFFER_LIMIT 30
#define BUFFMUTEX "prodConsBufferMutex"
#define ITEMS "prodConsItemsSem"
#define FINISH "prodConsFinishSem"
static tMutex buffMutex;
static tSem items;
static tSem finish;
static char * buffer;
static int bufferIndex;
static int running;
static char monsterCount;
static char sushiMenCount;

void initProdCons() {
    running = 1;
    char initialized = 0;
    sushiMenCount = 1;
    monsterCount = 1;
    buffer = malloc(500);
    bufferIndex = 0;
    buffMutex = createMutex(BUFFMUTEX);
    items = createSem(ITEMS);
    finish = createSem(FINISH);
    cleanScreen();
    printf("   ,,,,,,,,,,,\n");
    printf(",'' ;  ;  ;  ''|||///\n");
    printf("',,_;__;__;__;,'''\\\\\\\n");
    printf(" |            |\n");
    printf("  '.________.'\n");
    printf(".................................Welcome to: agodio/itba-so:1.0 SuhiBar.......................................\n");
    printf("Initially there is one shushiMan and one eating monster\n");
    printf("There is a maximum amount of sushi's that can be on the table, that is 30\n");
    printf("The sushimen will produce a random amount of sushi while the monster will eat a random amount of sushi\n");
    printf("If there is no place on the table the sushiman will not produce more sushi until space is available. Likewise the monster will not eat sushi if the table is empty he will wait until the sushiman produces more\n");
    printf("\nPress q to quit - when all process finish their task it will quit\n");
    printf("Press s to add a sushiman (max 5)\n");
    printf("Press m to add an eating monster (max 5)\n");
    printf("Press i to start\n");
    printf("\n");
    printf("Sushi Status Bar:\n");
    printf("empty                                                                                                       full\n");
    printf(" 1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  24  25  26  27  28  29  30\n");
    while (running) {
        char key;
        _syscall(_read, &key);
        switch (key) {
            case 's':
                if ((sushiMenCount < MAXSUSHIMEN) && initialized) {
                    sushiMenCount++;
                    exec(sushiMan, 0, 0);
                }
                break;
            case 'm':
                if ((monsterCount < MAXMONSTERS) && initialized) {
                    monsterCount++;
                    exec(monster, 0, 0);
                }
                break;
            case 'q':
                running = 0;
                break;
            case 'i':
                initialized = 1;
                exec(monster, 0, 0);
                exec(sushiMan, 0, 0);
                break;
        }
    }
    for (int i = 0; i < (sushiMenCount + monsterCount); i++) {
        wait(finish);
    }
    printf("\nGame Over\n");
    destroyMutex(buffMutex);
    destroySemaphore(finish);
    destroySemaphore(items);
    return;
}


int myRand() {
    static int next = 12;
    next = ((next * next) / 100 ) % 10000;
    return next ;
}

int myRandInRange (int min, int max) {
    return myRand() % (max+1-min) + min ;
}

unsigned int lfsr113_Bits (void)
{
    static unsigned int z1 = 12345, z2 = 12345, z3 = 12345, z4 = 12345;
    unsigned int b;
    b  = ((z1 << 6) ^ z1) >> 13;
    z1 = ((z1 & 4294967294U) << 18) ^ b;
    b  = ((z2 << 2) ^ z2) >> 27;
    z2 = ((z2 & 4294967288U) << 2) ^ b;
    b  = ((z3 << 13) ^ z3) >> 21;
    z3 = ((z3 & 4294967280U) << 7) ^ b;
    b  = ((z4 << 3) ^ z4) >> 12;
    z4 = ((z4 & 4294967168U) << 13) ^ b;
    return (z1 ^ z2 ^ z3 ^ z4);
}

unsigned short lfsr = 0xACE1u;
unsigned bit;

unsigned rand()
{
    bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
    return lfsr =  (lfsr >> 1) | (bit << 15);
}

void deleteSushi(int n) {
    for (int i =0; i < n; i++) {
        deleteChar();
    }
}

void sushiMan() {
    while(running) {

        adquire(buffMutex);

        int i = (lfsr113_Bits()%10)+1;
        //printf("sushi %d ", i);

        while ((i > 0) && (bufferIndex < BUFFER_LIMIT)) {
            if (bufferIndex >= 8) {
                printf("(@) ");

            } else {
                printf("(@)");
            }
            i--;
            bufferIndex++;
            delay(5000);
        }
        post(items);
        release(buffMutex);
    }
    post(finish);
    //killCurrentProcess();
}

void monster() {
    while(running) {

        wait(items);
        adquire(buffMutex);

        int i = (lfsr113_Bits()%10) + 1;
        // printf("moster %d ", i);
        while ((i > 0) && (bufferIndex > 0)) {
            if (bufferIndex > 8) {
                deleteSushi(4);
            } else {
                deleteSushi(3);
            }
            i--;
            bufferIndex--;
            delay(5000);
        }

        release(buffMutex);

    }
    post(finish);
    //killCurrentProcess();
}
