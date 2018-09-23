#include <stdint.h>
#include "TimeModule.h"
#include "systemCall.h"
#define WRITE 1
#define STDOUT 1

char * getTime() {
    static char buf[8] = {0};
    int aux;
    systemCall(WRITE, STDOUT, 6, 0, &aux, 0);
    aux -= 3;
    if (aux < 0) {
        aux += 24;
    }
    uintToBase(aux, buf, 10);
    if (buf[1] == 0) {
        buf[1] = buf[0];
        buf[0] = '0';
    }
    buf[2] = ':';
    systemCall(WRITE, STDOUT, 6, 1, &aux, 0);
    uintToBase(aux, buf+3, 10);
    if (buf[4] == 0) {
        buf[4] = buf[3];
        buf[3] = '0';
    }
    buf[5] = ':';
    systemCall(WRITE, STDOUT, 6, 2, &aux, 0);
    uintToBase(aux, buf + 6, 10);
    if (buf[7] == 0) {
        buf[7] = buf[6];
        buf[6] = '0';
    }
    return buf;
}

