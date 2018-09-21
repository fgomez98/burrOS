#ifndef IDTLOADER_H_
#define IDTLOADER_H_

#include <stdint.h>

/*COntrols the creation and management of system calls*/
void load_idt();
void _syscall_handler();

#endif
