//
//  mutex.h
//  TPARQUI
//
//  Created by Fermin Gomez on 9/10/18.
//
//

#ifndef semaphore_h
#define semaphore_h

#include <stdio.h>
#include "scheduler.h"
#include "queueADT.h"
#include "BuddyAllocationSystem.h"
#include "String.h"
#include "mutex.h"

typedef struct sem {
    mutex * lock;
    char * name;
    int value;
    queueADT queue;
} sem;

/**
 Crea un mutex dado un nombre y retorna un puntero al mismo
 */
sem * initSem(char * semName);

/**
 Se fija si el mutex con ese nombre existe, si existe retorna un puntero a el sino crea un mutex dado un nombre y retorna un puntero al mismo
 */
void destroySem(sem * s);

/**
 Destruye todos los mutex liberando el espacio de memoria reservado
 */
void destroyAllSem();

/**
 Elmina el mutex y  libera sus recursos
 */
sem * getSem(char * semName);

/**
 El proceso que llama toma el mutex
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este sera bloqueado
 funcion atomica
 */

void wait(sem * s);
/**
 El proceso que es dueño del mutex es el unico que puede liberarlo
 si algun otro proceso llama a la funcion y no es el dueño actual del mismo este no sera liberado
 funcion atomica
 */
void post(sem * s);

#endif /* semaphore_h */
