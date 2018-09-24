#ifndef BuddyAllocationSystem_h
#define BuddyAllocationSystem_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "VideoDriver.h"
#include "lib.h"
#include "String.h"

/**
  Desc: Libera un bloque de memoria (* memoryAdr) que fue previamente alocado por "malloc" asi este es disponible para otro uso.
  Ret: -
 */
void freeMemory(void * memoryAdr);

/**
 Desc: Reserva un bloque de memoria continguo de tamaño (request) para uso exclusivo del usuario, no se garantiza que si el usuario se pasa del limite maximo de su memoria este sea frenado por lo que queda a responsabilidad del usuario respetar los limites pedidos.
 Ret: Direccion del bloque de memoria
 */
void * mallocMemory(size_t request);

/**
 Desc: Reserva un bloque de memoria continguo de tamaño (request) para uso exclusivo del usuario, no se garantiza que si el usuario se pasa del limite maximo de su memoria este sea frenado por lo que queda a responsabilidad del usuario respetar los limites pedidos.
 Toda la informacion contenida en (* memorySrc) es copiada al nuevo bloque de memoria
 Ret: Direccion de nuevo bloque de memoria
 */
void * reallocMemory(void * memorySrc, size_t request);

/**
 Desc: Reserva un bloque de memoria continguo de tamaño (request) donde todo el espacio esta seteado en 0 (cero) para uso exclusivo del usuario, no se garantiza que si el usuario se pasa del limite maximo de su memoria este sea frenado por lo que queda a responsabilidad del usuario respetar los limites pedidos.
 Ret: Direccion del bloque de memoria
 */
void * callocMemory(size_t request);
/**
 Desc: Imprime en pantalla el estado actula de la memoria, mostrando que bloques estan dispoibles y cuales no.
 Ret: -
 */
void dumpMemory();

void put();

void sprintMemory(char* buffer, int buffSize);

#endif /* BuddyAllocationSystem */
