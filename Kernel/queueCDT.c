#include "queueADT.h"
#include "VideoDriver.h"

Colour colour3 = {100, 1000, 255};
queueADT newQueue(size_t bytes, int (*cmp)(void *, void *)) {
    queueADT new = mallocMemory(sizeof(*new));
    new->bytes = bytes;
    new->first = NULL;
    new->last = NULL;
    new->cmp = cmp;
    new->dim = 0;
    return new;
}

static
TNode * insertInOrderRec(TNode * current, queueADT q, void * elem) {
    if (current == NULL) {
        TNode * new = mallocMemory(sizeof(*new));
        memcpy(new->elem, elem, q->bytes);
        q->last = new;
        return new;
    } if ((*q->cmp)(current->elem, elem) < 0) {
        TNode * new = mallocMemory(q->bytes);
        memcpy(current->elem, elem, q->bytes);
        new->next = current;
        return new;
    }
    current->next = insertInOrderRec(current->next, q, elem);
    return current;
}

void insertInOrder(queueADT q, void * elem) {
    q->first = insertInOrderRec(q->first, q, elem);
}

void push(queueADT q, void * elem) {
    if (q == NULL) {
        return;
    }
    q->dim++;
    if (q->first == NULL) {
        TNode * new = mallocMemory(sizeof(*new));
        new->elem = elem;
        new->next = NULL;
        q->first = q->last = new;
        return;
    } else {
        TNode * new = mallocMemory(sizeof(*new));
        new->elem = elem;
        new->next = NULL;
        q->last->next = new;
        q->last = new;
        return;
    }
}

void * pop(queueADT q) {
    if (q == NULL || q->first == NULL) {
        return NULL;
    }
    q->dim--;
    TNode * aux = q->first->next;
    void * out = q->first->elem;
    freeMemory(q->first);
    q->first = aux;
    return out;
}

int getSize(queueADT q) {
    return q->dim;
}

static
TNode * removeREC(TNode * current, TNode * prev, void * elem, queueADT q, void * aux, char * found) {
    if (current == NULL) {
        return NULL;
        putStr("LLEGUE AL FINAL", colour3);
    } else if ((*(q->cmp))(current->elem, elem) == 0) {
        putStr("IGUALES", colour3);
        *(found) = 1;
        memcpy(aux, current->elem, q->bytes);
        if (current == q->last) {
            q->last = prev;
        }
        TNode * next = current->next;
        freeMemory(current->elem);
        freeMemory(current);
        return next;;
    } else {
        current->next = removeREC(current->next, current, elem, q, aux, found);
        return current;
    }
}

void * removeElem(queueADT q, void * elem) {
    if (q == NULL || q->cmp == NULL) {
        return NULL;
    }
    void * aux = mallocMemory(q->bytes);
    char found = 0;
    q->first = removeREC(q->first, NULL, elem, q, aux, &found);
    if (found) {
        return aux;
    } else {
        freeMemory(aux);
        return NULL;
    }
}

static
int belongsREC(TNode * node, void * elem, int (*cmp)(void *, void *)) {
    if (node == NULL) { // llego al final y no lo encontro
        return 0;
    } if (!(*cmp)(node->elem, elem)) { // si son iguales entonces lo encontre
        return 1;
    } else
        return 0 + belongsREC(node->next, elem, cmp);
}

int belongsElem(queueADT q, void * elem) {
    if (q == NULL) {
        return 0;
    }
    if (q->cmp == NULL) {
        return 0;
    }
    return belongsREC(q->first, elem, q->cmp);
}

void freeQueue(queueADT q) {
    if (q == NULL) {
        return;
    }
    TNode * queue = q->first;
    while (queue != NULL) {
        TNode * aux = queue;
        queue = queue->next;
        freeMemory(aux->elem);
        freeMemory(aux);
    }
    freeMemory(q);
}
