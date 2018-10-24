#include "filofochos.h"
#define MIDX XRESOLUTION/2
#define MIDY YRESOLUTION/2
#define abs(x) (((x)<0) ? -(x) : (x))
#define MAX_FILOSOPHERS 6
#define THINKING 2
#define HUNGRY 1
#define EATING 0
#define FILOMUTEX "philosophersMutex"
#define FINISH "philosophersFinishSem"
char * semNames[MAX_FILOSOPHERS] = {"philosophersSem1", "philosophersSem2", "philosophersSem3", "philosophersSem4", "philosophersSem5", "philosophersSem6"};
static tMutex mutex;
static tSem sem[MAX_FILOSOPHERS];
//static tSem finish;
static int running;
static int filofochosAmount;
static int goToSleep;

static Colour white = {255, 255, 255};
static Colour black = {0,0,0};

static Colour filofochoColours[] = {
    {100, 100, 255},
    {180, 40, 18},
    {100, 1000, 255},
    {50, 50, 280},
    {70, 0, 94},
    {255, 165, 0}
};

int philState[MAX_FILOSOPHERS] = {THINKING, THINKING, THINKING, THINKING, THINKING, THINKING};

static Colour forks[MAX_FILOSOPHERS] = {
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
    {255, 255, 255},
};

void philospher(int id);

//TODO: hace falta mostrar cuadno los filosofos tienen hambre??, se pone medio inentendible el dibujo
void initFilofochos() {
    running = 1;
    filofochosAmount = 0;
    goToSleep = 0;
    char initialized = 0;
    mutex = createMutex(FILOMUTEX);
//    finish = createSem(FINISH);
//    char waitCant = 0;
    for (int i = 0; i < MAX_FILOSOPHERS; i++) {
        forks[i] = white;
    }
    for (int i = 0; i < MAX_FILOSOPHERS; i++) {
        sem[i] = createSem(semNames[i]);
    }
    for (int i = 0; i < MAX_FILOSOPHERS; i++) {
        philState[i] = THINKING;
    }
    cleanScreen();
    printf("Welcome to Dinning Philosofers\n");
    printf("Initially there are 2 philosofers\n");
    printf("Each philospher is represented with a colour on the table\n");
    printf("when a philosopher is hungry an empty plate will apear on it otherwise it will be full of food\n");
    printf("forks are coloured with the colour of the philopher thar owns them if no philosopher is using them their colour  will be white\n");
    printf("Press q to quit\n");
//    printf("Press z to add a philosofer to the table (max 6)\n");
//    printf("Press x to kill a philosofer\n");
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
                drawTable();
                adquire(mutex);
                cleanScreen();
                printf("Press z to add a philosofer to the table (max 6)\n");
                printf("Press x to kill a philosofer\n");
                printf("Press q to quit\n");
                exec("philosopher", philospher, filofochosAmount++, 0);
                exec("philosopher", philospher, filofochosAmount++, 0);
                release(mutex);
                drawTable();
                break;
        }

    }
    while (running) {
        char key;
        _syscall(_read, &key);
        switch (key) {
            case 'z':
                if (filofochosAmount < MAX_FILOSOPHERS) {
                    wait(sem[0]); // espero a que deje el tenedor el de la derecha
                    adquire(mutex);
                    clearTable();
                    exec("philosopher", philospher, filofochosAmount++, 0);
                    drawTable();
                    release(mutex);
                    post(sem[0]);
                }
                break;
            case 'x':
                if (filofochosAmount > 2) {
                    goToSleep = 1;
                }
                break;
            case 'q':
//                waitCant = filofochosAmount;
                running = 0;
                break;
            }
    }
//    for (int i = 0; i < waitCant; i++) {
////        wait(finish);
////        drawTable();
//    }
    cleanScreen();
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
    delay(10000);
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
        drawFilofochos();
        drawForks();
        post(sem[id]);
    } 
}

void take_fork(int id) {
    adquire(mutex);
    philState[id] = HUNGRY;
    drawFilofochos();
    test(id);
    release(mutex);
    wait(sem[id]);
}

void put_fork(int id) {
    adquire(mutex);
    philState[id] = THINKING;
    unMarkForks(id);
    drawFilofochos();
    drawForks();
    test(left(id));
    test(right(id));
    release(mutex);
}

void philospher(int id) {
    drawTable();
    while (running && !(goToSleep && id == (filofochosAmount-1))) {
        think();
        take_fork(id);
        eat();
        put_fork(id);
    }
        // al hacer put fork tmb lo marca en thinking y el fork en blanco
    adquire(mutex);
    goToSleep = 0;
    clearTable();
    filofochosAmount--;
    drawTable();
    release(mutex);
   if (!running) {
//       post(finish);
       clearTable();
    }
}

void drawFilofocho(Colour colour, int x, int y, int state) {
    DrawFilledCircle(x, y, 35, colour); // cuando esta en thinking se muestra solemente un circulo del color que representa al filosofo
    if (state == EATING) {
        DrawFilledCircle(x, y, 15, white);
    } else if (state == HUNGRY) {
        drawCircle(x, y, 15, white);
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
            drawFilofocho(filofochoColours[4], MIDX - 180, MIDY - 80, philState[4]);
            drawFilofocho(filofochoColours[3], MIDX, MIDY - 215, philState[3]);
            break;
        case 6:
            drawFilofocho(filofochoColours[0], MIDX, MIDY + 215, philState[0]);
            drawFilofocho(filofochoColours[5], MIDX - 160, MIDY + 140, philState[5]);
            drawFilofocho(filofochoColours[1], MIDX + 160, MIDY + 140, philState[1]);
            drawFilofocho(filofochoColours[2], MIDX + 180, MIDY - 80, philState[2]);
            drawFilofocho(filofochoColours[4], MIDX - 180, MIDY - 80, philState[4]);
            drawFilofocho(filofochoColours[3], MIDX, MIDY - 215, philState[3]);
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
        case 6:
            drawFilofocho(black, MIDX, MIDY + 215, -1);
            drawFilofocho(black, MIDX - 160, MIDY + 140, -1);
            drawFilofocho(black, MIDX + 160, MIDY + 140, -1);
            drawFilofocho(black, MIDX + 180, MIDY - 80, -1);
            drawFilofocho(black, MIDX - 180, MIDY - 80, -1);
            drawFilofocho(black, MIDX, MIDY - 215, -1);
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
            drawFork(forks[4], MIDX - 250, MIDX - 170, MIDY - 40, MIDY - 10);
            drawFork(forks[3], MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
            break;
        case 6:
            drawFork(forks[0], MIDX + 50, MIDX + 50, MIDY + 250, MIDY + 180);
            drawFork(forks[5], MIDX - 130, MIDX - 80, MIDY + 200, MIDY + 140);
            drawFork(forks[1], MIDX + 250, MIDX + 170, MIDY + 100, MIDY + 70);
            drawFork(forks[2], MIDX + 130, MIDX + 80, MIDY - 160, MIDY - 100);
            drawFork(forks[4], MIDX - 250, MIDX - 170, MIDY - 40, MIDY - 10);
            drawFork(forks[3], MIDX - 50, MIDX - 50, MIDY - 250, MIDY - 180);
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
        case 6:
            drawFork(black, MIDX + 50, MIDX + 50, MIDY + 250, MIDY + 180);
            drawFork(black, MIDX - 130, MIDX - 80, MIDY + 200, MIDY + 140);
            drawFork(black, MIDX + 250, MIDX + 170, MIDY + 100, MIDY + 70);
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
