#include <mutex.h>
#include <stdint.h>
#include <scheduler.h>
#include <BuddyAllocationSystem.h>
#include <String.h>
#include <pipe.h>
#include <queueADT.h>
#include <VideoDriver.h>
#define BUFFERSIZE 1024

struct pipe_t{
    char * name;
    mutex * useMutex;
    mutex * readMutex;
    mutex * writeMutex;
    int waitingForRead;
    int waitingForWrite;
    char buffer[BUFFERSIZE];
    unsigned int readPosition;
    unsigned int writePosition;
};

struct pipeNode{
    struct pipe_t * pipe;
    struct pipeNode * next;
};

pipeNode * pipeList = NULL;


/*BORRAR*/
Colour yellows = {100, 1000, 255};

pipe_t * pipe(char * name) {
    if (name == NULL)
        return NULL;

    pipeNode * aux = pipeList;
    while(aux != NULL) {
        if(strcmp(aux->pipe->name, name))
            return aux->pipe;
        aux = aux->next;
    }

    pipe_t * newPipe = mallocMemory(sizeof(pipe_t));
    if(newPipe == NULL)
        return NULL;

    char namecpy[strlenght(name) +1];
    newPipe->name = strcpy(namecpy,name);
    char readMutexName[strlenght(name) + 6];
    char writeMutexName[strlenght(name) + 6];
    char useMutexName[strlenght(name) + 6];

    newPipe->readMutex = initMutex(strconcat(name, " read",readMutexName));
    newPipe->writeMutex = initMutex(strconcat(name, " write",writeMutexName));
    newPipe->useMutex = initMutex(strconcat(name, " use",useMutexName));
    newPipe->readPosition = 0;
    newPipe->writePosition = 0;
    newPipe->waitingForRead = -1;
    newPipe->waitingForWrite = -1;

    pipeNode * newNode = mallocMemory(sizeof(pipeNode));

    if(newNode == NULL)
        return NULL;

    newNode->pipe = newPipe;
    newNode->next = pipeList;
    pipeList = newNode;

    return newPipe;
}

int exists(pipe_t * pipe) {
    pipeNode * aux = pipeList;
    while(aux != NULL) {
        if(strcmp(aux->pipe->name, pipe->name) == 0)
            return 1;
        aux = aux->next;
    }
    return 0;
}

void unlinkPipe(char * name) {
    pipeList = unlinkPipeR(pipeList, name);
}

pipeNode * unlinkPipeR(pipeNode * node, char * name) {
    if(node == NULL)
        return NULL;

    if(strcmp(node->pipe->name, name) == 0) {
        pipeNode * aux = node->next;
        destroyMutex(node->pipe->useMutex);
        destroyMutex(node->pipe->readMutex);
        destroyMutex(node->pipe->writeMutex);
        freeMemory(node->pipe->name);
        freeMemory(node->pipe);
        return aux;
    }
    node->next = unlinkPipeR(node->next, name);
    return node;
}

int readPipe(pipe_t * pipe, char * resp, size_t ammount) {
  /*  if(!exists(pipe))
        return -1;
*/
    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;


    adquire(pipe->readMutex);
    adquire(pipe->useMutex);
    int blocked = 0;
    int readBytes = BUFFERSIZE - getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE);
    if(readBytes <= 0) {
        pipe->waitingForRead = getRunningPid();
        blocked = 1;
        putStr("me bloqueo leyendo\n", yellows);
        release(pipe->useMutex);
        blockProcess(pipe->waitingForRead);
        putStr("Me desbloquearon\n", yellows);
        pipe->waitingForRead = -1;
    }

    if(blocked ==1)
        adquire(pipe->useMutex);
    int i;
    for(i = 0; i < ammount ; i++, (pipe->readPosition)++) {
        if(pipe->readPosition == 1024)
            pipe->readPosition = 0;
        resp[i] = (pipe->buffer)[pipe->readPosition];
    }

    if(pipe->waitingForWrite != -1)
        unblockProcess(pipe->waitingForWrite);

    release(pipe->useMutex);
    release(pipe->readMutex);

    return i;
}

int writePipe(pipe_t * pipe, char * msg, size_t ammount){
    /*if(!exists(pipe))
        return -1;
*/
    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    adquire(pipe->writeMutex);
    int blocked = 0;
    int hasSpaceToWrite = 0;

    while(!hasSpaceToWrite) {
        adquire(pipe->useMutex);
        if (getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE) >= ammount) {
            hasSpaceToWrite = 1;
        }

        if(!hasSpaceToWrite){
            pipe->waitingForWrite = getRunningPid();
            blocked = 1;
            putStr("me bloqueo\n", yellows);
            release(pipe->useMutex);
            blockProcess(pipe->waitingForWrite);
        }
    }
    pipe->waitingForWrite = -1;

    for(int i = 0; i < ammount; i++, (pipe->writePosition)++) {
        if(pipe->writePosition == 1024)
            pipe->writePosition = 0;
        (pipe->buffer)[pipe->writePosition] = msg[i];
    }

    if(pipe->waitingForRead != -1)
        unblockProcess(pipe->waitingForRead);


    release(pipe->useMutex);
    release(pipe->writeMutex);

    return ammount;
}

int getAvailableToWrite(int start, int final, int size) {
    if(final >= start)
        return size - (final - start);
    else {
        return start - final;
    }
}

int readAvailable(pipe_t * pipe){
    if(BUFFERSIZE - getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE) > 0)
        return 1;
    return 0;
}

int cmp(int * pid1, int * pid2) {
    return *pid1-*pid2;
}