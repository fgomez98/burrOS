////
////  mutex.c
////  TPARQUI
////
////  Created by Fermin Gomez on 9/10/18.
////
////
//
//#include "mutex.h"
//
//
//typedef struct mutex {
//    char value;
//    queueADT queue;
//} mutex;
//
//mutex * initMutex() {
//    mutex * mutex;
//    mutex->queue = newQueue(sizeof(int));
//    mutex->value = 1;
//    return mutex;
//}
//
//void acquire(mutex * m) {
//    //IRQ_OFF; // interrupciones deshabilitadas
//    
//    if (m->value == 0) { // esta tomado
//        //add(m->queue, getMyPid());
//        //block(getMyPid());
//    }
//    
//    m->value = 0;
//    
//    //IRQ_RES; // interrupciones habilitadas
//}
//
//void release(mutex * m) {
//   // IRQ_OFF; // interrupciones deshabilitadas
//    
//    if (m->value == 1) {
//        return;
//    }
//    
//    m->value = 1;
//    //wakeUp(*(get(m->queue)));
//    
//    //IRQ_RES; // interrupciones habilitadas
//}
