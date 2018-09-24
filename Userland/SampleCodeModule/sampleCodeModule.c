#include <stdint.h>
#include <stdarg.h>
#include"stdio.h"
#include "systemCall.h"
#include "shell.h"
#define WRITE 1
#define READ 0
#define STDOUT 1
#define STDIN 0
#define CLEAR 0

void printf(char* fmt, ...);

int main() {

    initializeShell();

	return 0;
}
