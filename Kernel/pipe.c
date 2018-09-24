#include <mutex.h>
#include <stdint.h>
#include <scheduler.h>
#include <BuddyAllocationSystem.h>
#include <String.h>
#include <pipe.h>
#include <queueADT.h>
#define BUFFERSIZE 1024

struct pipe_t{
    char * name;
    mutex * useMutex;
    mutex * readMutex;
    mutex * writeMutex;
    queueADT waitReadQueue;
    queueADT waitWriteQueue;
    char buffer[BUFFERSIZE];
    unsigned int readPosition;
    unsigned int writePosition;
};

struct pipeNode{
    struct pipe_t * pipe;
    struct pipeNode * next;
};

pipeNode * pipeList = NULL;

pipe_t * pipe(char * name) {
    if (name == NULL)
        return NULL;

    pipe_t * newPipe = mallocMemory(sizeof(pipe_t));
    if(newPipe == NULL)
        return NULL;

    char namecpy[strlenght(name) +1];
    strcpy(namecpy,name);
    newPipe->name = strcpy(namecpy,name);
    char readMutexName[strlenght(name) + 6];
    char writeMutexName[strlenght(name) + 6];
    char useMutexName[strlenght(name) + 6];

    newPipe->readMutex = initMutex(strconcat(name, " read",readMutexName));
    newPipe->writeMutex = initMutex(strconcat(name, " write",writeMutexName));
    newPipe->useMutex = initMutex(strconcat(name, " use",useMutexName));
    newPipe->readPosition = 0;
    newPipe->writePosition = 0;
    newPipe->waitReadQueue = newQueue(sizeof(int),cmp);
    newPipe->waitWriteQueue = newQueue(sizeof(int),cmp);

    pipeNode * newNode = mallocMemory(sizeof(pipeNode));

    if(newNode == NULL)
        return NULL;

    newNode->pipe = newPipe;
    newNode->next = pipeList;
    pipeList = newNode;

    return newPipe;
}

void unlinkPipe(char * name) {
    pipeList = unlinkPipeR(pipeList, name);
}

pipeNode * unlinkPipeR(pipeNode * node, char * name) {
    if(node->pipe == NULL)
        return NULL;
    if(strcmp(node->pipe->name, name) == 0) {
        pipeNode * aux = node->next;
        freeMemory(node->pipe->name);
        destroyMutex(node->pipe->useMutex);
        destroyMutex(node->pipe->readMutex);
        destroyMutex(node->pipe->writeMutex);
        freeMemory(node->pipe);
        return aux;
    }
    node->next = unlinkPipeR(node->next, name);
    return node;
}

int readPipe(pipe_t * pipe, char * resp, size_t ammount) {

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    adquire(pipe->readMutex);
    adquire(pipe->useMutex);
    int blocked = 0;
    int readBytes = BUFFERSIZE - getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE);
    if(readBytes <= 0) {
        int runningPid = getRunningPid();
        push(pipe->waitReadQueue, &runningPid);
        blocked = 1;
        release(pipe->useMutex);
        blockProcess(runningPid);
    }

    if(blocked ==1)
        adquire(pipe->useMutex);
    int availableToRead = BUFFERSIZE - getAvailableToWrite(pipe->writePosition, pipe->readPosition, BUFFERSIZE);
    int i;
    for(i = 0; i < availableToRead ; i++, (pipe->readPosition)++) {
        if(pipe->readPosition == 1024)
            pipe->readPosition = 0;
        resp[i] = (pipe->buffer)[pipe->readPosition];
    }
    release(pipe->useMutex);
    release(pipe->readMutex);

    if(getSize(pipe->waitWriteQueue) > 0) {
        int * pid = (int *)pop(pipe->waitWriteQueue);
        unblockProcess(*pid);
    }

    return i;
}

int writePipe(pipe_t * pipe, char * msg, size_t ammount){

    if(ammount > BUFFERSIZE)
        ammount = BUFFERSIZE;

    adquire(pipe->writeMutex);

    int blocked = 0;
    int hasSpaceToWrite = 0;
    while(!hasSpaceToWrite) {
        adquire(pipe->useMutex);
        if(getAvailableToWrite(pipe->readPosition, pipe->writePosition, BUFFERSIZE) < ammount)
            hasSpaceToWrite = 1;

        if(!hasSpaceToWrite){
            int runningPid = getRunningPid();
            push(pipe->waitWriteQueue, &runningPid);
            blocked = 1;
            release(pipe->useMutex);
            blockProcess(runningPid);
        }
    }

    for(int i = 0; i < ammount; i++, (pipe->writePosition)++) {
        if(pipe->writePosition == 1024)
            pipe->writePosition = 0;
        (pipe->buffer)[pipe->writePosition] = msg[i];
    }
    release(pipe->useMutex);
    release(pipe->writeMutex);

    if(getSize(pipe->waitReadQueue) > 0) {
        int * pid = (int *)pop(pipe->waitReadQueue);
        unblockProcess(*pid);
    }

    return ammount;
}

int getAvailableToWrite(int start, int final, int size) {
    if(final >= start)
        return size - (final - start);
    else {
        return start - final;
    }
}

int cmp(int * pid1, int * pid2) {
    return *pid1-*pid2;
}