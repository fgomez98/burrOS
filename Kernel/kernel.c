#include "VideoDriver.h"
#include <stdint.h>
//#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include "idtLoader.h"
#include "time.h"
//#include "String.h"
#include "Keyboard.h"
#include "KeyboardDriver.h"
#include "ScanCodes.h"
#include "BuddyAllocationSystem.h"
#include "process.h"
#include "scheduler.h"
extern uint64_t getStack();
extern uint64_t rip;
extern uint64_t stack;
extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;
extern uint64_t dir;
extern uint64_t stack;


static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;




typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}
static char buff[8];
int main() {
    Colour colour = {255, 255, 255};
    stack = getStack();
    rip = (uint64_t)sampleCodeModuleAddress;
//   putStr("Base adress: ", colour);
//    uintToBase(&endOfKernel, buff, 10);
//    putStr(buff, colour);
//    putChar('\n', colour);
//    put();
//    dumpMemory();

    load_idt();
    init_(sampleCodeModuleAddress);
    //    ((EntryPoint)sampleCodeModuleAddress)();

	return 0;
}
