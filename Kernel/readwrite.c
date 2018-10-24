#include <mutex.h>
#include <stdint.h>
#include <scheduler.h>
#include <BuddyAllocationSystem.h>
#include <String.h>
#include <VideoDriver.h>
#include <lib.h>
#include <process.h>
#include <readwrite.h>

struct fileDecryptorStruct {
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
};

mutex * listMutex;
linkedList fdList = NULL;
Colour whiteColor = {255, 255, 255};


int fdcmp(fileDecryptor * f1, fileDecryptor * f2){
    if(f1 == NULL || f2 == NULL)
        return -1;
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
fileDecryptor * getFd(linkedList fdList, int fd) {
    struct fileDecryptorStruct aux;
    aux.fd = fd;
    adquire(listMutex);
    fileDecryptor * resp = getElemFromList(fdList, &aux);
    release(listMutex);
    return resp;
}

static
void createPipeLink(int fd1, int fd2){
    open(fd1);
    open(fd2);
    fileDecryptor * myfd = getFd(fdList,fd1);
    myfd->pipefd = fd2;
    myfd = getFd(fdList,fd2);
    myfd->pipefd = fd1;
    return 1;
}

static
int addUserToFd(fileDecryptor * myfd){
    int runningPid = getRunningPid();
    adquire(listMutex);
    if(!containsList(myfd->users, runningPid)) {
        addToList(myfd->users, runningPid);
        release(listMutex);
        return 1;
    }
    release(listMutex);
    return 0;
}

static
int pipeAlreadyExists(fileDecryptor * myfd1, fileDecryptor * myfd2) {
    if(myfd1->pipefd == myfd2->fd && myfd2->pipefd == myfd1->fd)
        return 1;
    return 0;
}

//ESTO LO LLAMARIA EL KERNEL ANTES CUANDO CREA LOS BUFFERS DE OUTPUT, ETC
void initializeFileDecryptors() {
    fdList = newList(sizeof(fileDecryptor *), fdcmp);
    listMutex = initMutex("fdListMutex");
}

int pipe(int fd[]) {
    if(fd == NULL || fd[0] == NULL || fd[1] == NULL || fd[0] == 1 || fd[1] == 1 || fd[0] == 0 ||fd[1] == 0)
        return -1;

    fileDecryptor * myfd1 = getFd(fdList, fd[0]);
    fileDecryptor * myfd2 = getFd(fdList, fd[1]);
    if(myfd1 == NULL || myfd2 == NULL) {
        if(myfd1 != NULL && myfd1->pipefd != -1)
            return -1;
        if(myfd2 != NULL && myfd2->pipefd != -1)
            return -1;

        createPipeLink(fd[0], fd[1]);
        return 1;
    }
    else if(pipeAlreadyExists(myfd1,myfd2)) {
        addUserToFd(myfd1);
        addUserToFd(myfd2);
        return 1;
    }
    else if(myfd1->pipefd == -1 && myfd2->pipefd == -1) {

        myfd1->pipefd = myfd2->fd;
        myfd2->pipefd = myfd1->fd;

        addUserToFd(myfd1);
        addUserToFd(myfd2);
        return 1;
    }
    return -1;
}

int open(int fd){

    if(fd == 0 || fd == 1)
        return -1;

    fileDecryptor * newfd = getFd(fdList, fd);
    if(newfd != NULL){
        int runningPid = getRunningPid();
        return addUserToFd(newfd);
    }

    newfd = mallocMemory(sizeof(newfd));
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

    adquire(listMutex);
    addToList(fdList, newfd);
    release(listMutex);

    return 1;
}

int openWithPid(int fd, int pid) {
    
    if(fd == 0 || fd == 1)
        return -1;
    
    fileDecryptor * newfd = getFd(fdList, fd);
    if(newfd != NULL){
        int runningPid = pid;
        return addUserToFd(newfd);
    }
    
    newfd = mallocMemory(sizeof(newfd));
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
    
    addToList(newfd->users, pid);
    
    adquire(listMutex);
    addToList(fdList, newfd);
    release(listMutex);
    
    return 1;
}

//ESTO VUELA , LO DEJO POR LAS DUDAS
void printfd(){
    char buffer[19];
    int i = 0;
    fileDecryptor * resp = getElemAtFromList(fdList, i);
    while(resp != NULL){
        intToString(buffer,resp->fd);
        putStr(buffer,whiteColor);
        i++;
        resp = getElemAtFromList(fdList, i);
    }

}

int close(int fd) {
    fileDecryptor * myfd = getFd(fdList, fd);
    if(removeElemList(myfd->users, getRunningPid()) == NULL)
        return 0;
    if(myfd->pipefd == -1 && getListSize(myfd->users) == 0) {
        freeMemory(myfd->buffer);
        freeList(myfd->users);
        adquire(listMutex);
        removeElemList(fdList, myfd);
        release(listMutex);
        return 1;
    }

    fileDecryptor * reader = myfd;
    if(myfd->pipefd != -1)
        reader = getFd(fdList, myfd->pipefd);

    if(getListSize(myfd->users) == 1 && containsList(myfd->users, reader->waitingForRead) == 1) {
        unblockProcess(myfd->waitingForRead);
        putStr("unblock 17a",whiteColor);
    }
    return 1;

}

int read(int fd, char * msg, int amount) {
    if (fd == 0) {
        int processStdInFd = getRunningProcess()->stdIn;
        if(processStdInFd == 0) {
            char key;
            int i = 0;
            while ((key = getKeyInput()) != 0 && (i < amount)) {
                msg[i] = key;
                i++;
            }
            return i;
        }
    } else {
        myfd = getFd(fdList, fd);
        if (myfd == NULL)
            return -1;

        if (!containsList(myfd->users, getRunningPid()))
            return -1;
    }

    if(amount > BUFFERSIZE)
        amount = BUFFERSIZE;

    adquire(myfd->readMutex);
    adquire(myfd->useMutex);

    /* Si no hay nada para escribir y no hay nadie que escriba, o hay alguien y soy yo no me bloqueo*/

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
        if(myfd->readPosition == BUFFERSIZE)
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

    fileDecryptor * myfd;

    if(fd == 1){
        int processStdOutFd = getRunningProcess()->stdOut;
        if(processStdOutFd == 1) {
            putStrWithSize(msg, whiteColor, amount);
            return amount;
        }
        else{
            myfd = getFd(fdList, processStdOutFd);
            if(!containsList(myfd->users, getRunningPid()))
                return -1;
        }
    }
    else {
        myfd = getFd(fdList, fd);

        if (myfd == NULL)
            return -1;

        if(!containsList(myfd->users, getRunningPid()))
            return -1;

        if (myfd->pipefd != -1) {
            myfd = getFd(fdList, myfd->pipefd);
            if (myfd == NULL)
                return -1;
        }
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
        if (myfd->writePosition == BUFFERSIZE)
            myfd->writePosition = 0;
        (myfd->buffer)[myfd->writePosition] = msg[i];
    }


    if(myfd->waitingForRead != -1)
        unblockProcess(myfd->waitingForRead);

    release(myfd->useMutex);
    release(myfd->writeMutex);

    return amount;
}


void dup2(int newFd, int fdToReplace) {
    if (newFd == fdToReplace || (newFd < 2 && newFd >= 0)) {
        return;
    }

    open(newFd);
    if (fdToReplace == 0) {
        getRunningProcess()->stdIn = newFd;
    }
    else if(fdToReplace == 1){
        getRunningProcess()->stdOut = newFd;
    }
}


