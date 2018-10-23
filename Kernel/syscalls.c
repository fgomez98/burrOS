#include "VideoDriver.h"
#include "Keyboard.h"
#include <stdint.h>
#include "math.h"
#include "Beep.h"
#include "TimeDriver.h"
#include "BuddyAllocationSystem.h"
#include "scheduler.h"
#include "String.h"
#include "mutex.h"
#include "semaphore.h"
#include "syscalls.h"
#include "pipe.h"
#include "readwrite.h"
#include <process.h>


static char buff[8];
Colour colourM = {255,255,255};

typedef uint64_t(*systemCall)();


systemCall sysCalls[] = { 0, 0, 0,
        (systemCall) _clearScreen,
		(systemCall) _read,
        (systemCall) _beep,
		(systemCall) _unbeep,
		(systemCall) _ps,
		(systemCall) _sprintMemory,
		(systemCall) _deleteChar,
		(systemCall) _putChar,
		(systemCall) _putString,
		(systemCall) _malloc,
		(systemCall) _calloc,
		(systemCall) _realloc,
		(systemCall) _free,
		(systemCall) _exec,
		(systemCall) _kill,
		(systemCall) _killCurrentProcess,
		(systemCall) _createMutex,
		(systemCall) _destroyMutex,
		(systemCall) _adquireMutex,
		(systemCall) _releaseMutex,
		(systemCall) _createSem,
		(systemCall) _destroySem,
		(systemCall) _semWait,
		(systemCall) _semPost,
		(systemCall) _getHour,
		(systemCall) _getMin,
		(systemCall) _getSec,
		(systemCall) _drawPixel,
		(systemCall) _namedPipe,
		(systemCall) _destroyPipe,
		(systemCall) _readPipe,
		(systemCall) _writePipe,
		(systemCall) _open,
		(systemCall) _readfd,
		(systemCall) _write,
		(systemCall) _close,
		(systemCall) _pipe,
		(systemCall) _dup,

};

void syscall_handler(uint64_t index, uint64_t a, uint64_t b, uint64_t c, uint64_t d, uint64_t e) {
	sysCalls[index](a, b, c, d , e);
}

void _clearScreen(){
	modeScreen();
}

void _read(uint64_t key){
	char * p =(char *) key;
	*(p) =  getKeyInput();
}

void _beep(){
	beep();
}

void _unbeep(){
	unBeep();
}

void _ps(char* buffer, uint64_t size){
	sprintProcesses(buffer, size);
}

void _sprintMemory(char* buffer, uint64_t size){
	sprintMemory(buffer, size);
}

void _deleteChar(){
	deleteChar(); //deletes the next available character
}

void _putChar(int c){
	  putChar(c, colourM);//puts the char recieved by a char pointer on screen, in white
}

void _putString(char* string){
	  putStr(string, colourM); //puts the string recieved by a char pointer on screen, in white
}

void _malloc(uint64_t bytes, uint64_t address){
	void** pp;
	pp = (void **)address;
	*pp =  mallocMemoryInProcess(bytes, getRunningProcess());
}

void _calloc(uint64_t bytes, uint64_t address) {
	void** pp;
	pp = (void **)address;
	*pp =  callocMemoryInProcess(bytes, getRunningProcess());
}

void _realloc(uint64_t bytes, uint64_t address){
	void** pp;
	pp = (void **)address;
	*pp =  reallocMemoryInProcess(bytes, getRunningProcess(), address);
}

void _free(uint64_t ad){
	freeMemoryInProcess(ad, getRunningProcess());
}

void _exec(uint64_t pName,uint64_t startingPoint, int *  pid, int argc, void* argv[]){
	tProcess* process = createProcess(pName, startingPoint, 0, argc, argv);
    *pid = process->pid;
    addProcess(process);
}

void _kill(uint64_t pid, uint64_t message){
	switch(message){
		case 0:
			endProcess(pid);
		break;
		case 1:
			blockProcess(pid);
		break;
		case 2:
			unblockProcess(pid);
		break;
	}
}

void _killCurrentProcess(){
	endProcess(getRunningPid());
}

void _createMutex(uint64_t mutexName, uint64_t mutex){
	uint64_t* ui;
	ui = (uint64_t*)mutex;
	*ui = getMutex(mutexName);
}

void _destroyMutex(uint64_t mu){
	mutex* m ;
	m = (mutex*) mu;
	destroyMutex(m);
}

void _adquireMutex(uint64_t mu){
	mutex* m ;
	m = (mutex*) mu;
	adquire(m);
}

void _releaseMutex(uint64_t mu){
	mutex* m ;
	m = (mutex*) mu;
  release(m);
}

void _createSem(uint64_t semName, uint64_t semaph ){
	uint64_t* ui;
	ui = (uint64_t*)semaph;
	*ui = getSem(semName);
}

void _destroySem(uint64_t semaph){
	sem* s;
	s = (sem*) semaph;
	destroySem(s);
}

void _semWait(uint64_t semaph){
	sem* s;
	s = (sem*) semaph;
	wait(s);
}

void _semPost(uint64_t semaph){
	sem* s;
	s = (sem*) semaph;
	post(s);
}

void _getHour(uint64_t arg){
	int * p = (int *) arg;
    *(p) = getHour();
}

void _getMin(uint64_t arg){
	int * p = (int *) arg;
    *(p) = getMin();
}

void _getSec(uint64_t arg){
	int * p = (int *) arg;
    *(p) = getSec();
}

void _drawPixel(uint64_t x, uint64_t y, uint64_t rgb){
	Colour colour2 = intToRGB(rgb); // changes colour to GRB format
	putPixel(x, y, colour2); //puts pixel on screen in arg4 (x) and arg5 (y) positions, in selected colour
}


void _namedPipe(uint64_t name, uint64_t ** p){
	*p = namedPipe(name);
}

void _destroyPipe(uint64_t name){
   destroyPipe(name);
}

uint64_t _readPipe(uint64_t pipe, uint64_t a, uint64_t resp, uint64_t amount){
	return readPipe(pipe, resp, amount);

}

uint64_t _writePipe(uint64_t * pipe, uint64_t * a, uint64_t msg, uint64_t amount){
    return writePipe(pipe, msg, amount);
}

uint64_t _open(uint64_t fd) {
    return open(fd);
}

uint64_t _readfd(uint64_t fd, uint64_t * msg, uint64_t amount) {
    return read(fd,msg,amount);
}

uint64_t _write(uint64_t fd, uint64_t * msg, uint64_t amount) {
    return write(fd,msg,amount);
}

uint64_t _close(uint64_t fd) {
	return close(fd);
}

uint64_t _pipe(uint64_t fd[]) {
	return pipe(fd);
}

// hay que ver que devuelve dup2
uint64_t _dup(uint64_t newFd, uint64_t oldFd) {
	//dup2(newFd, oldFd);
	return 1;
}