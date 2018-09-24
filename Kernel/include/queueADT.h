#ifndef queueADT_h
#define queueADT_h

#include "BuddyAllocationSystem.h"
#include "lib.h"

typedef struct TNode {
    struct TNode * next;
    void * elem;
} TNode;

/**
 Creaa una queue genericas de elementos de tamañ bytes y recive una funciona coparadora (con esta queue podemos uitlizar las funciones belongs y remove) si no se va a utilizar pasar NULL
 */

typedef struct queueCDT {
    size_t bytes;
    int (*cmp)(void *, void *);
    TNode * last;
    TNode * first;
    int dim;
} queueCDT;

typedef queueCDT* queueADT;

queueADT newQueue(size_t bytes, int (*cmp)(void *, void *));

/**
 Libera los recursos de la queue (q)
 */
void freeQueue(queueADT q);

/**
 Agrega un elemento (* elem) al final de la queue (q) o al principio si esta vacia con shallow copy
 */
void push(queueADT q, void * elem);

/**
 Quita el primer elemento de la queue (q)
 */
void * pop(queueADT q);

/**
 Remueve un elemento (* elem) de la queue (q)
 Se podra utilizar si es que posee una funcion comparadora
 */
void * removeElem(queueADT q, void * elem);

/**
 Retorna 1 si un elemento (* elem) pertenece a la queue (q) 0 sino.
 Se podra utilizar si es que posee una funcion comparadora
 */
int belongsElem(queueADT q, void * elem);

/**
 Retonra la cantidad de elementos en la cola
 */
int getSize(queueADT q);

/**
 Retonra el puntero al elemento en la cola
 */
void * getElem(queueADT q, void * elem);

#endif