#include "BuddyAllocationSystem.h"




typedef struct list_t {
    struct list_t * next, * prev;
} list_t;

static uint8_t * baseAdress;
extern uint8_t endOfKernel;
static char splitBlocks[array_size(1 << (BUCKET_AMOUNT - 1))] = {0};
static char allocatedBlocks[array_size(1 << (BUCKET_AMOUNT))] = {0};
static list_t * buckets[BUCKET_AMOUNT];
static char buff[8];
static Colour colour = {255, 255, 255};

int indexInLevelOf(uint8_t * memoryAdr, int level) {
    return((memoryAdr - baseAdress) >> (29 - level)) + (1 << level) - 1;
}

int adress(int index, int level) {
    return baseAdress + ((index - (1 << level) + 1) << (29 - level));
}

void put() {
    putStr("size: ", colour);
    uintToBase((1 << (BUCKET_AMOUNT)), buff, 10);
    putStr(buff, colour);

    putStr("BitArraySize: ", colour);
    uintToBase(array_size(1 << (BUCKET_AMOUNT)), buff, 10);
    putStr(buff, colour);
}

void init() {
    baseAdress = (uint8_t *)&endOfKernel;
    list_t * node = (list_t *)baseAdress;
    node->next = NULL;
    putStr("Base adress: ", colour);
    uintToBase(baseAdress, buff, 10);
    putStr(buff, colour);
    putStr(" Total Size: ", colour);
    uintToBase(total_size, buff, 10);
    putStr(buff, colour);
    putChar('\n', colour);
    buckets[0] = node;
}

int bucketRequest(size_t request) {
    int bucket = BUCKET_AMOUNT - 1;
    int size = LEAF_SIZE;
    while (size < request) {
        bucket--;
        size *= 2;
    }
    return bucket;
}

list_t * popList(int level) {
    list_t * aux = buckets[level];
    buckets[level] = aux->next;
    buckets[level]->prev = NULL;
    return aux;
}

void pushList(int level, uint8_t * memoryAdr) {
    list_t * node = (list_t *)memoryAdr;
    node->next = buckets[level];
    node->prev = NULL;
    buckets[level]->prev = node;
    buckets[level] = node;
}

void removeFromList(int level, list_t * node) {
    if (node == buckets[level]) {
        popList(level);
        return;
    }
    list_t * next = node->next;
    list_t * prev = node->prev;
    if (prev != NULL) {
        prev->next = next;
    } if ( next != NULL) {
        next->prev = prev;
    }

}

int blockHasBeenSplit(uint8_t * memoryAdr, int level) {
    int index = indexInLevelOf(memoryAdr, level);
    return get_bit(splitBlocks, index);
}

int findLevel(uint8_t * memoryAdr) {
    int n = BUCKET_AMOUNT- 1;
    while (n > 0) {
        if (blockHasBeenSplit(memoryAdr, (n-1))) {
            return n;
        }
        n = n - 1;
    }
    return 0;
}

int isParentSplit(int index) {
    return get_bit(splitBlocks, move_to_parent(index));
}

void flipParentSplit(int index) {
    toggle_bit(splitBlocks, move_to_parent(index));
}

void * mallocMemory(size_t request) {
    if (request > size_of_level(0)) {
        return NULL;
    }
    if (baseAdress == NULL) {
        init();
    }
    int level = bucketRequest(request);
    if (!getNextFreeBlock(level)) {
        return NULL;
    }
    list_t * list = popList(level);
    int index = indexInLevelOf((uint8_t *)list, level);
    toggle_bit(allocatedBlocks, index);
    return (void *)list;
}

int getNextFreeBlock(int level) {
    int i = level;
    while ((buckets[i] == NULL) && (i >=0)) {
        i--;
    }
    if (i < 0) {
        return FALSE;
    }
    while (i < level) {
        splitBlock(i++);
    }
    return TRUE;
}

void splitBlock(int level) {
    list_t * aux = popList(level);
    int index = indexInLevelOf(aux, level);
    toggle_bit(splitBlocks,index);
    pushList(level+1, adress(move_to_right_child(index), level+1));
    pushList(level+1, adress(move_to_left_child(index), level+1));
}

// TODO: hay que chequear el rango de la direccion de memoria??
void freeMemory(void * memoryAdr) {

    if ((memoryAdr == NULL) || (memoryAdr < baseAdress) || (memoryAdr > (baseAdress+ total_size))) {
        return;
    }
    int level = findLevel(memoryAdr);
    int index = indexInLevelOf(memoryAdr, level);
    toggle_bit(allocatedBlocks, index);

    while (index > 0) {
        if (get_bit(allocatedBlocks, move_to_sibling(index)) || get_bit(splitBlocks, move_to_sibling(index))) {
            break;
        }
        removeFromList(level, (list_t *)adress(move_to_sibling(index), level));
        index = move_to_parent(index);
        toggle_bit(splitBlocks,index);
        level--;
    }
    pushList(level, adress(index, level));

}

void * reallocMemory(void * memorySrc, size_t request) {
    int level = findLevel(memorySrc);
    char size = size_of_level(level);
    if (size >= request) {
        return memorySrc;
    }
    char * memoryAdr = mallocMemory(request);
    for (int i = 0; i < size; i++) {
         *(memoryAdr+i) = *((char *)memorySrc+i);
    }
    freeMemory(memorySrc);
    return memoryAdr;
}

void * reallocMemoryAlreadyFreed(void * memorySrc, size_t request) {
    int level = findLevel(memorySrc);
    char size = size_of_level(level);
    if (size >= request) {
        return memorySrc;
    }
    char * memoryAdr = mallocMemory(request);
    for (int i = 0; i < size; i++) {
         *(memoryAdr+i) = *((char *)memorySrc+i);
    }
    return memoryAdr;
}

void * callocMemory(size_t request) {
    char * memoryAdr = mallocMemory(request);
    for (int i = 0; i < request; i++) {
        *(memoryAdr+i) = 0;
    }
    return memoryAdr;
}

void dumpMemory() {
    int index = 0;
    int level = 0;
    int maxIndex = 0;
    while (level < BUCKET_AMOUNT) {
        maxIndex += max_blocks_of_level(level);
        putStr("Level: ", colour);
        uintToBase(level, buff, 10);
        putStr(buff, colour);
        putChar('\n', colour);
        while (index < maxIndex) {
//            uintToBase(index, buff, 10);
//            putStr(" -- ", colour);
//            putStr(buff, colour);
            if (!is_leaf_block(index)) {
                if (get_bit(splitBlocks,index)) {
                    uintToBase(index, buff, 10);
                    putStr(" -- ", colour);
                    putStr(buff, colour);
                    putStr(" is Split", colour);
                    putStr(" --", colour);
                    putChar('\n', colour);
                    index++;
                    continue;
                }
            }
            if (get_bit(allocatedBlocks, index)) {
                uintToBase(index, buff, 10);
                putStr(" -- ", colour);
                putStr(buff, colour);
                putStr(" is Allocated", colour);
                putStr(" --", colour);
                putChar('\n', colour);
            }
//          else {
//            putStr(" is Free", colour);
//           }
            index++;
        }
//        putStr(" --", colour);
//        putChar('\n', colour);
        level++;
    }
    putChar('\n', colour);
}


void sprintMemory(char* buffer, int buffSize){
  int i = 0;
  int occ;
  occ = strcpy2(buffer+i,"block   baseAddress  finalAddress\n",buffSize);
  i += occ;
  buffSize -= occ;
  uint64_t base;
  uint64_t limit;

  int index = 0;
  int level = 0;
  int maxIndex = 0;

  while (level < BUCKET_AMOUNT) {
      maxIndex += max_blocks_of_level(level);
      while (index < maxIndex) {
          if (get_bit(allocatedBlocks, index)) {
              base = adress(index, level);
              limit = base + size_of_level(level);

              uintToBase(index, buff, 10);
              occ = strcpy2(buffer+i, buff ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

              occ = strcpy2(buffer+i, "    " ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

              uintToBase(base, buff, 10);
              occ = strcpy2(buffer+i, buff ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

              occ = strcpy2(buffer+i, "    " ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

              uintToBase(limit, buff, 10);
              occ = strcpy2(buffer+i, buff ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

              occ = strcpy2(buffer+i, "\n" ,buffSize);
              i += occ;
              buffSize -= occ;
              if(buffSize<=0) break;

          }
          index++;
      }
      level++;
  }
  occ = strcpy2(buffer+i, "\n" ,buffSize);

}
