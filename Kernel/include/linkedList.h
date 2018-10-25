#include "BuddyAllocationSystem.h"
#include "lib.h"

typedef struct ListNode {
    struct ListNode * next;
    void * elem;
} ListNode;

typedef struct linkedListStruct {
    size_t bytes;
    int (*cmp)(void *, void *);
    ListNode * last;
    ListNode * first;
    int dim;
} linkedListStruct;

typedef linkedListStruct * linkedList;

linkedList newList(size_t bytes, int (*cmp)(void *, void *));

void addToList(linkedList l, void * elem);

void * getElemAtFromList(linkedList l, int i);

void * getElemFromList(linkedList l, void * elem);

void replaceElemAt(linkedList l, void * elem, int i);

void replaceElem(linkedList l, void * newElem, void * oldElem);

int getListSize(linkedList l);

void * removeElemList(linkedList l, void * elem);

int removeElemListAt(linkedList l, int i);

int containsList(linkedList l, void * elem);

void freeList(linkedList l);

void freeListNodes(linkedList l);

