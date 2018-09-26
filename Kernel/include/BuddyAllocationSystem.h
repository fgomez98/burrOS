#ifndef BuddyAllocationSystem_h
#define BuddyAllocationSystem_h

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "VideoDriver.h"
#include "lib.h"
#include "String.h"

#define TRUE 1
#define FALSE 0
#define BUCKET_AMOUNT 23
#define LEAF_SIZE 128

#define max_blocks_of_level(n)  ((1<<(n)))
#define is_leaf_block(n) ((1 << (BUCKET_AMOUNT - 1)) <= (n)) ? TRUE : FALSE
#define move_to_parent(index) (((index) - 1) / 2)
#define move_to_left_child(index) ((index) * 2 + 1)
#define move_to_right_child(index) ((index) * 2 + 2)
#define move_to_sibling(index) ((((index) - 1) ^ 1) + 1)
#define array_size(i) (((i) / 8) + (!! ((i) % 8)))
#define toggle_bit(array, i) (array[i / 8] ^= 1 << (i % 8))
#define get_bit(array, i) (1 & (array[i / 8] >> (i % 8)))
#define total_size (1<<(BUCKET_AMOUNT-1)) * LEAF_SIZE // es como hacer :(1<<30) = 1*(2^30
#define size_of_level(n) (total_size / (1<<(n)))
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

void * reallocMemoryAlreadyFreed(void * memorySrc, size_t request);

#endif /* BuddyAllocationSystem */
