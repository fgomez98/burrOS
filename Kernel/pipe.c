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

Colour color = {255,0,0};

pipe_t * pipe(char * name) {
    if (name == NULL)
        return NULL;

    pipeNode * aux = pipeList;
    while(aux != NULL) {
        if(strcmp(aux->pipe->name, name)) {
            putStr("Ya existia", color);
            return aux->pipe;
        }
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
    putStr("Cree el pipe", color);
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

void destroyPipe(char * name) {
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
    if(!exists(pipe))
        return -1;

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    adquire(pipe->readMutex);
    adquire(pipe->useMutex);


    int readBytes = availableToRead(pipe);
    if(readBytes <= 0) {
        int pid = getRunningPid();
        pipe->waitingForRead = pid;
        release(pipe->useMutex);
        putStr("me bloqueo", color);
        blockProcess(pid);
        pipe->waitingForRead = -1;
        adquire(pipe->useMutex);
    }


    int i;
    for(i = 0; i < ammount && availableToRead(pipe) > 0 ; i++, (pipe->readPosition)++) {
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
    if(!exists(pipe))
        return -1;

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    adquire(pipe->writeMutex);
    int blocked = 0;
    int hasSpaceToWrite = 0;

    while(!hasSpaceToWrite) {
        adquire(pipe->useMutex);
        if (availableToWrite(pipe) >= ammount) {
            hasSpaceToWrite = 1;
        }

        if(!hasSpaceToWrite){
            int pid = getRunningPid();
            pipe->waitingForWrite = pid;
            blocked = 1;
            release(pipe->useMutex);
            blockProcess(pid);
            pipe->waitingForWrite = -1;
        }
    }

    for(int i = 0; i < ammount; i++, (pipe->writePosition)++) {
        if(pipe->writePosition == 1024)
            pipe->writePosition = 0;
        (pipe->buffer)[pipe->writePosition] = msg[i];

    if(pipe->waitingForRead != -1)
        unblockProcess(pipe->waitingForRead);
    }
    release(pipe->useMutex);
    release(pipe->writeMutex);

    return ammount;
}

int availableToWrite(pipe_t * pipe) {
    if(pipe->writePosition >= pipe->readPosition)
        return BUFFERSIZE - (pipe->writePosition - pipe->readPosition);
    else {
        return BUFFERSIZE - (pipe->readPosition - pipe->writePosition);
    }
}

int availableToRead(pipe_t * pipe){
    return BUFFERSIZE - availableToWrite(pipe);
}

int cmp(int * pid1, int * pid2) {
    return *pid1-*pid2;
}
