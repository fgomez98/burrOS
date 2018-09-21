#include "VideoDriver.h"

#define ZERO_EXCEPTION_ID 0
#define INVALIDOPCODE_EXCEPTION_ID 1

static void zero_division(uint64_t *stackPointer);
static void invalid_opcode(uint64_t *stackPointer);
void printIpAndRegs(uint64_t* stackPointer);

void exceptionDispatcher(uint64_t exception, uint64_t *stackPointer) {
	switch(exception){
		case ZERO_EXCEPTION_ID:
			zero_division(stackPointer);
			//como hice pushstate antes de llamar a la handler, tengo todos los registros
			//a partir de donde empieza el stackPointer
		break;
		case INVALIDOPCODE_EXCEPTION_ID:
			invalid_opcode(stackPointer);
		break;
	}

}

static void zero_division(uint64_t *stackPointer) {
	Colour colour;
	colour.Red = 255;
	colour.Green = 255;
	colour.Blue = 255;
	newLine();

	putStr("ERROR: DIVISION POR CERO", colour);
	printIpAndRegs(stackPointer);

}

static void invalid_opcode(uint64_t *stackPointer){
	Colour colour;
	colour.Red = 255;
	colour.Green = 255;
	colour.Blue = 255;
		newLine();
	putStr("ERROR: CODIGO DE OPERACION INVALIDO\n", colour);
	printIpAndRegs(stackPointer);


}


void printIpAndRegs(uint64_t* stackPointer){
	Colour colour;
	colour.Red = 255;
	colour.Green = 255;
	colour.Blue = 255;

	 char* regs[]= {"RAX ","RBX ","RCX ","RDX ","RBP ","RDI ","RSI ","R8 ","R9 ","R10 ","R11 ","R12 ","R13 ","R14 ","R15  "};

	 /*stack frame tiene la direccion del
	 r15. adelante esta r14 y asi sucesivamente. uno antes de r15 esta la direccion
	 de retorno de la handler que seria el rip. */

	 int j=0;
	 int i=0;
	for (i =14, j=0; i>=0; i--, j++){
		newLine();
		putStr(regs[i], colour);
		putHexa(stackPointer[j], colour);
	}
	newLine();
	putStr("RIP ", colour);
	putHexa(stackPointer[j], colour);
	newLine();

}
