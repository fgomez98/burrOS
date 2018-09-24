//
//  mutex.h
//  TPARQUI
//
//  Created by Fermin Gomez on 9/10/18.
//
//

#ifndef mutex_h
#define mutex_h

#include <stdio.h>
#include "scheduler.h"
#include "queueADT.h"
#include "BuddyAllocationSystem.h"

typedef struct mutex {
    int mutex_holder;
    char * name;
    char value;
    queueADT queue;
} mutex;

/**
 Crea un mutex dado un nombre y retorna un puntero al mismo
 */
mutex * initMutex(char * mutexName);

/**
 Se fija si el mutex con ese nombre existe, si existe retorna un puntero a el sino crea un mutex dado un nombre y retorna un puntero al mismo
 */
void destroyMutex(mutex * m);

/**
 Elmina el mutex y  libera sus recursos
 */
mutex * getMutex(char * mutexName);

/**
 El proceso que llama toma el mutex
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este sera bloqueado
 funcion atomica
 */

void adquire(mutex * m);
/**
 El proceso que es dueño del mutex es el unico que puede liberarlo
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este no sera liberado
 funcion atomica
 */
void release(mutex * m);

#endif /* mutex_h */
