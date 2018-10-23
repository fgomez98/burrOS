#include <lib.h>

#ifndef SYSCALLS_H
#define SYSCALLS_H



void syscall_handler(uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);
void _clearScreen();
void _read(uint64_t key);
void _beep();
void _unbeep();
void _ps(char* buffer, uint64_t size);
void _sprintMemory(char* buffer, uint64_t size);
void _deleteChar();
void _putChar(int c);
void _putString(char* string);
void _malloc(uint64_t bytes, uint64_t address);
void _calloc(uint64_t bytes, uint64_t address);
void _realloc(uint64_t bytes, uint64_t address);
void _free(uint64_t ad);
void _exec(uint64_t pName,uint64_t startingPoint, int *pid, int argc, void* argv[]);
void _kill(uint64_t pid, uint64_t message);
void _killCurrentProcess();
void _createMutex(uint64_t mutexName, uint64_t mutex);
void _destroyMutex(uint64_t mutex);
void _adquireMutex(uint64_t mutex);
void _releaseMutex(uint64_t mutex);
void _createSem(uint64_t semName, uint64_t sem);
void _destroySem(uint64_t sem);
void _semWait(uint64_t semaph);
void _semPost(uint64_t semaph);
void _getHour(uint64_t arg);
void _getMin(uint64_t arg);
void _getSec(uint64_t arg);
void _drawPixel(uint64_t x, uint64_t y, uint64_t rgb);
void _namedPipe(uint64_t name, uint64_t ** p);
void _destroyPipe(uint64_t name);
uint64_t _readPipe(uint64_t pipe, uint64_t a, uint64_t resp, uint64_t amount);
uint64_t _writePipe(uint64_t * pipe, uint64_t * a, uint64_t msg, uint64_t amount);
uint64_t _open(uint64_t fd);
uint64_t _readfd(uint64_t fd, uint64_t * msg, uint64_t amount);
uint64_t _write(uint64_t fd, uint64_t * msg, uint64_t amount);
uint64_t _close(uint64_t fd);
uint64_t _pipe(uint64_t fd[]);
uint64_t _dup(uint64_t newFd, uint64_t oldFd);


#endif
