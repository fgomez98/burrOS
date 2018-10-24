#include <stdint.h>
#ifndef SYSCALL_H
#define SYSCALL_H

#define _clearScreen 3
#define _read 4
#define _beep 5
#define _unbeep 6
#define _ps 7
#define _sprintMemory 8
#define _deleteChar 9
#define _putChar 10
#define _putString 11
#define _malloc 12
#define _calloc 13
#define _realloc 14
#define _free 15
#define _exec 16
#define _kill 17
#define _killCurrentProcess 18
#define _createMutex 19
#define _destroyMutex 20
#define _adquireMutex 21
#define _releaseMutex 22
#define _createSem 23
#define _destroySem 24
#define _semWait 25
#define _semPost 26
#define _getHour 27
#define _getMin 28
#define _getSec 29
#define _drawPixel 30
#define _pipe 31
#define _destroyPipe 32
#define _readPipe 33
#define _writePipe 34
#define _open 35
#define _readfd 36
#define _write 37
#define _close 38
#define _pipe 39
#define _dup 40
#define _getPid 41
#define _nice 42

uint64_t _syscall(uint64_t, ...);

#endif
