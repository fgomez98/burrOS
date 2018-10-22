#include <mutex.h>
#include <stdint.h>
#include <scheduler.h>
#include <BuddyAllocationSystem.h>
#include <String.h>
#include <VideoDriver.h>
#include <lib.h>

#define BUFFERSIZE 1024

typedef struct fileDecryptor {
    int fd;
    char * buffer;

    // para que sea mas rapido podria guardar el fd o el char *
    int pipefd; // a que fd tiene que escribir si hay un pipe

    mutex * useMutex;
    mutex * readMutex;
    mutex * writeMutex;
    int waitingForRead;
    int waitingForWrite;

    unsigned int readPosition;
    unsigned int writePosition;

    linkedList users;
}fileDecryptor;

linkedList fdList = NULL;
Colour somecolor = {255, 255, 255};

static
int fdcmp(fileDecryptor * f1, fileDecryptor * f2){
    if(f1 == NULL || f2 == NULL)
        return -1;
    else
        return f1->fd - f2->fd;
}

static
int availableToWrite(fileDecryptor * fd) {
    if(fd->writePosition >= fd->readPosition)
        return BUFFERSIZE - (fd->writePosition - fd->readPosition);
    else {
        return BUFFERSIZE - (fd->readPosition - fd->writePosition);
    }
}

static
int availableToRead(fileDecryptor * fd){
    return BUFFERSIZE - availableToWrite(fd);
}

static
fileDecryptor * getFd(linkedList * fdList, int fd) {
    struct fileDecryptor aux;
    aux.fd = fd;
    return getElemFromList(fdList, &aux);
}

static
void createPipeLink(int fd1, int fd2){
    fileDecryptor * newfd = mallocMemory(sizeof(newfd));
    if(newfd == NULL)
        return -1;
    newfd->fd = fd1;
    newfd->buffer = mallocMemory(BUFFERSIZE);
    newfd->pipefd = fd2;
    addToList(newfd->users, getRunningPid());


    char * name = mallocMemory(19);
    if(name == NULL)
        return -1;
    intToString(name, fd1);

    char readMutexName[strlenght(name) + 6];
    char writeMutexName[strlenght(name) + 6];
    char useMutexName[strlenght(name) + 6];

    newfd->readMutex = initMutex(strconcat(name, " read",readMutexName));
    newfd->writeMutex = initMutex(strconcat(name, " write",writeMutexName));
    newfd->useMutex = initMutex(strconcat(name, " use",useMutexName));
    newfd->readPosition = 0;
    newfd->writePosition = 0;
    newfd->waitingForRead = -1;
    newfd->waitingForWrite = -1;
    newfd->users = newList(sizeof(int), pidcmp);
    addToList(newfd->users, getRunningPid());

    addToList(fdList, newfd);
    return 1;
}

//ESTO LO LLAMARIA EL KERNEL ANTES CUANDO CREA LOS BUFFERS DE OUTPUT, ETC
void initializeFileDecryptors() {
    fdList = newList(sizeof(fileDecryptor), fdcmp);
}

int pipe(int fd[]) {
    if(fd == NULL || fd[0] == NULL || fd[1] == NULL)
        return -1;

    fileDecryptor * myfd = getFd(fdList, fd[0]);
    if(myfd != NULL)
        return -1;
    else {
        myfd = getFd(fdList, fd[1]);
        if(myfd != NULL)
            return -1;
    }

    createPipeLink(fd[0], fd[1]);
    createPipeLink(fd[1], fd[0]);
}

int open(int fd){
    // Si ya existe solo agrego el pid
    fileDecryptor * newfd = getFd(fdList, fd);
    if(newfd != NULL){
        int runningPid = getRunningPid();
        if(!containsList(newfd->users, runningPid)) {
            addToList(newfd->users, runningPid);
            addFdToProcess(getRunningProcess(), fd);
            return 1;
        }
        return 0;
    }

    newfd = mallocMemory(sizeof(fileDecryptor));
    if(newfd == NULL)
        return -1;


    char * name = mallocMemory(19);
    intToString(name, fd);

    char * readMutexName = mallocMemory(strlenght(name) + 6);
    char * writeMutexName = mallocMemory(strlenght(name) + 6);
    char * useMutexName = mallocMemory(strlenght(name) + 6);

    newfd->useMutex = initMutex(strconcat(name, " use",useMutexName));
    newfd->readMutex = initMutex(strconcat(name, " read",readMutexName));
    newfd->writeMutex = initMutex(strconcat(name, " write",writeMutexName));

    newfd->fd = fd;
    newfd->buffer = mallocMemory(BUFFERSIZE);
    newfd->pipefd = -1;
    newfd->readPosition = 0;
    newfd->writePosition = 0;
    newfd->waitingForRead = -1;
    newfd->waitingForWrite = -1;

    newfd->users = newList(sizeof(int), pidcmp);
    addToList(newfd->users, getRunningPid());

    addToList(fdList, newfd);
    addFdToProcess(getRunningProcess(), fd);
    return 1;
}

int close(int fd) {
    fileDecryptor * myfd = getFd(fdList, fd);
    if(removeElemList(myfd->users, getRunningPid()) == NULL)
        return 0;
    if(getListSize(myfd->users) == 0) {
        freeMemory(myfd->buffer);
        freeList(myfd->users);
        removeElemList(fdList, myfd);
    }
    return 1;

}

int read(int fd, char * msg, int amount) {
    fileDecryptor * myfd = getFd(fdList, fd);
    if(myfd == NULL)
        return -1;

    fileDecryptor * writerFd = myfd;
    if(myfd->pipefd != -1) {
        writerFd = getElemFromList(fdList, myfd->pipefd);
    }

    if(amount > BUFFERSIZE)
        amount = BUFFERSIZE;

    adquire(myfd->readMutex);
    /* Si no hay nada para escribir y no hay nadie que escriba, o hay alguien y soy yo no me bloqueo*/
    if(availableToRead(myfd) == 0 && (getListSize(writerFd->users) == 0 || (getListSize(writerFd->users) == 1 && containsList(writerFd->users, getRunningPid())) )){
        release(myfd->readMutex);
        return 0;
    }
    adquire(myfd->useMutex);

    int readBytes = availableToRead(myfd);
    if(readBytes <= 0) {
        int pid = getRunningPid();
        myfd->waitingForRead = pid;
        release(myfd->useMutex);
        blockProcess(pid);
        myfd->waitingForRead = -1;
        adquire(myfd->useMutex);
    }

    int i;
    for(i = 0; i < amount && availableToRead(myfd) > 0 ; i++, (myfd->readPosition)++) {
        if(myfd->readPosition == 1024)
            myfd->readPosition = 0;
        msg[i] = (myfd->buffer)[myfd->readPosition];
    }
    if(myfd->waitingForWrite != -1)
        unblockProcess(myfd->waitingForWrite);

    release(myfd->useMutex);
    release(myfd->readMutex);

    return i;
}

int write(int fd, char * msg, int amount) {

    fileDecryptor * myfd = getFd(fdList, fd);
    if(myfd == NULL)
        return -1;

    if(!containsList(myfd->users,getRunningPid()))
        return -1;

    if(myfd->pipefd != -1){
        myfd = getFd(fdList, myfd->pipefd);
        if(myfd == NULL)
            return -1;
    }

    if(amount > BUFFERSIZE)
        amount = BUFFERSIZE;

    adquire(myfd->writeMutex);
    int hasSpaceToWrite = 0;

    while(!hasSpaceToWrite) {
        adquire(myfd->useMutex);
        if (availableToWrite(myfd) >= amount) {
            hasSpaceToWrite = 1;
        }

        if(!hasSpaceToWrite){
            int pid = getRunningPid();
            myfd->waitingForWrite = pid;
            release(myfd->useMutex);
            blockProcess(pid);
            myfd->waitingForWrite = -1;
        }
    }

    for(int i = 0; i < amount; i++, (myfd->writePosition)++) {
        if (myfd->writePosition == 1024)
            myfd->writePosition = 0;
        (myfd->buffer)[myfd->writePosition] = msg[i];
    }

    if(myfd->waitingForRead != -1)
        unblockProcess(myfd->waitingForRead);

    release(myfd->useMutex);
    release(myfd->writeMutex);

    return amount;
}

