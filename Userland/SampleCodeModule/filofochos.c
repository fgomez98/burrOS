#include "filofochos.h"
#define MIDX XRESOLUTION/2
#define MIDY YRESOLUTION/2
#define abs(x) (((x)<0) ? -(x) : (x))
#define MAX_FILOSOPHERS 5
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define FILOMUTEX "philosophersMutex"
char * semNames[] = {"philosophersSem1", "philosophersSem2", "philosophersSem3", "philosophersSem4", "philosophersSem5"};
static tMutex mutex;
static tSem sem[MAX_FILOSOPHERS];
static int running;
static int filofochosAmount;

static Colour white = {255, 255, 255};
static Colour black = {0,0,0};
static Colour filofochoColours[] = {
    {100, 100, 255},
    {180, 40, 18},
    {100, 1000, 255},
    {50, 50, 280},
    {70, 0, 94}
};
int philState[MAX_FILOSOPHERS];

static Colour forks[] = {
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255}
//    {100, 100, 255},
//    {180, 40, 18},
//    {100, 1000, 255},
//    {50, 50, 280},
//    {180, 180, 180}
};

void philospher(int id);

void initFilofochos() {
    running = 1;
    filofochosAmount = 0;
    char initialized = 0;
    mutex = createMutex(FILOMUTEX);
    for (int i = 0; i < MAX_FILOSOPHERS; i++) {
        sem[i] = createSem(semNames[i]);
    }
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
                exec("philosopher", philospher, filofochosAmount++, 0);
                exec("philosopher", philospher, filofochosAmount++, 0);
                break;
        }

    }
    while (running) {
        drawTable();
        char key;
        _syscall(_read, &key);
        switch (key) {
            case 'z':
                if (filofochosAmount < MAX_FILOSOPHERS) {
                    clearTable();
                    exec("philosopher", philospher, filofochosAmount++, 0);
                }
                break;
            case 'x':
//                if (filofochosAmount > 2) {
//                    clearTable();
//                    filofochosAmount--;
//                }
                break;
            case 'q':
                running = 0;
                break;
            }
    }
    //cleanScreen();
}

int left(int i) {
    return (i + filofochosAmount - 1) % filofochosAmount;
}

int right(int i) {
    return (i + 1) % filofochosAmount;
}

void think() {
    delay(9000);
}

void eat() {
    delay(9000);
}

void markForks(int id) {
    forks[left(id)] = filofochoColours[id];
    forks[id] = filofochoColours[id];
}

void unMarkForks(int id) {
    forks[left(id)] = white;
    forks[id] = white;
}

void test(int id) {
    if (philState[id] == HUNGRY && philState[left(id)] != EATING && philState[right(id)] != EATING) {
        philState[id] = EATING;
        markForks(id);
        post(sem[id]);
    } 
}

void take_fork(int id) {
    adquire(mutex);
    philState[id] = HUNGRY;
    test(id);
    release(mutex);
    wait(sem[id]);
}

void put_fork(int id) {
    adquire(mutex);
    philState[id] = THINKING;
    unMarkForks(id);
    test(left(id));
    test(right(id));
    release(mutex);
}

void philospher(int id) {
    while (running) {
        think();
        take_fork(id);
        eat();
        put_fork(id);
    } 
}

void drawFilofocho(Colour colour, int x, int y, int state) {
    DrawFilledCircle(x, y, 35, colour);
    if (state == EATING) {
        DrawFilledCircle(x, y, 15, white);
    }
}

void drawFork(Colour colour ,int x1, int y1, int x2, int y2) {
    line_fast(x1, x2, y1, y2, colour);
}

void drawFilofochos() {
    switch (filofochosAmount) {
        case 2:
            drawFilofocho(filofochoColours[0], MIDX, MIDY + 215, philState[0]);
            drawFilofocho(filofochoColours[1], MIDX, MIDY - 215, philState[1]);
            break;
        case 3:
            drawFilofocho(filofochoColours[0], MIDX - 150, MIDY + 150, philState[0]);
            drawFilofocho(filofochoColours[1], MIDX + 150, MIDY + 150, philState[1]);
            drawFilofocho(filofochoColours[2], MIDX, MIDY - 215, philState[2]);
            break;
        case 4:
            drawFilofocho(filofochoColours[0], MIDX - 150, MIDY + 150, philState[0]);
            drawFilofocho(filofochoColours[1], MIDX + 150, MIDY + 150, philState[1]);
            drawFilofocho(filofochoColours[2], MIDX + 150, MIDY - 150, philState[2]);
            drawFilofocho(filofochoColours[3], MIDX - 150, MIDY - 150, philState[3]);
            break;
        case 5:
            drawFilofocho(filofochoColours[0], MIDX - 160, MIDY + 170, philState[0]);
            drawFilofocho(filofochoColours[1], MIDX + 160, MIDY + 170, philState[1]);
            drawFilofocho(filofochoColours[2], MIDX + 180, MIDY - 80, philState[2]);
            drawFilofocho(filofochoColours[3], MIDX - 180, MIDY - 80, philState[3]);
            drawFilofocho(filofochoColours[4], MIDX, MIDY - 215, philState[4]);
            break;
    }
}

void clearFilofochos() {
    switch (filofochosAmount) {
        case 2:
            drawFilofocho(black, MIDX, MIDY + 215, -1);
            drawFilofocho(black, MIDX, MIDY - 215, -1);
            break;
        case 3:
            drawFilofocho(black, MIDX - 150, MIDY + 150, -1);
            drawFilofocho(black, MIDX + 150, MIDY + 150, -1);
            drawFilofocho(black, MIDX, MIDY -215, -1);
            break;
        case 4:
            drawFilofocho(black, MIDX - 150, MIDY + 150, -1);
            drawFilofocho(black, MIDX + 150, MIDY + 150, -1);
            drawFilofocho(black, MIDX - 150, MIDY - 150, -1);
            drawFilofocho(black, MIDX + 150, MIDY - 150, -1);
            break;
        case 5:
            drawFilofocho(black, MIDX, MIDY - 215, -1);
            drawFilofocho(black, MIDX - 160, MIDY + 170, -1);
            drawFilofocho(black, MIDX + 160, MIDY + 170, -1);
            drawFilofocho(black, MIDX - 180, MIDY - 80, -1);
            drawFilofocho(black, MIDX + 180, MIDY - 80, -1);
            break;
    }
}

void drawForks() {
    switch (filofochosAmount) {
        case 2:
            drawFork(forks[0], MIDX + 50, MIDX + 50, MIDY + 250, MIDY + 180);
            drawFork(forks[1], MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
        case 3:
            drawFork(forks[0], MIDX - 120, MIDX - 70, MIDY + 230, MIDY + 150);
            drawFork(forks[1], MIDX + 240, MIDX + 160, MIDY + 120, MIDY + 70);
            drawFork(forks[2], MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
        case 4:
            drawFork(forks[0], MIDX - 120, MIDX - 70, MIDY + 230, MIDY + 150);
            drawFork(forks[1], MIDX + 240, MIDX + 160, MIDY + 120, MIDY + 70);
            drawFork(forks[2], MIDX + 120, MIDX + 70, MIDY - 230, MIDY - 150);
            drawFork(forks[3], MIDX - 240, MIDX - 160, MIDY - 120, MIDY - 70);
            break;
        case 5:
            drawFork(forks[0], MIDX - 130, MIDX - 80, MIDY + 230, MIDY + 170);
            drawFork(forks[1], MIDX + 250, MIDX + 170, MIDY + 130, MIDY + 100);
            drawFork(forks[2], MIDX + 130, MIDX + 80, MIDY - 160, MIDY - 100);
            drawFork(forks[3], MIDX - 250, MIDX - 170, MIDY - 40, MIDY - 10);
            drawFork(forks[4], MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
    }
    
}

void clearForks() {
    switch (filofochosAmount) {
        case 2:
            drawFork(black, MIDX + 50, MIDX + 50, MIDY + 250, MIDY + 180);
            drawFork(black, MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
        case 3:
            drawFork(black, MIDX - 120, MIDX - 70, MIDY + 230, MIDY + 150);
            drawFork(black, MIDX + 240, MIDX + 160, MIDY + 120, MIDY + 70);
            drawFork(black, MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
        case 4:
            drawFork(black, MIDX - 120, MIDX - 70, MIDY + 230, MIDY + 150);
            drawFork(black, MIDX + 240, MIDX + 160, MIDY + 120, MIDY + 70);
            drawFork(black, MIDX + 120, MIDX + 70, MIDY - 230, MIDY - 150);
            drawFork(black, MIDX - 240, MIDX - 160, MIDY - 120, MIDY - 70);
            break;
        case 5:
            drawFork(black, MIDX - 130, MIDX - 80, MIDY + 230, MIDY + 170);
            drawFork(black, MIDX + 250, MIDX + 170, MIDY + 130, MIDY + 100);
            drawFork(black, MIDX + 130, MIDX + 80, MIDY - 160, MIDY - 100);
            drawFork(black, MIDX - 250, MIDX - 170, MIDY - 40, MIDY - 10);
            drawFork(black, MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
    }
    
}

void drawTable() {
    drawCircle(MIDX, MIDY, 300, white);
    drawFilofochos();
    drawForks();
}

void clearTable() {
    drawCircle(MIDX, MIDY, 300, black);
    clearFilofochos();
    clearForks();
}
