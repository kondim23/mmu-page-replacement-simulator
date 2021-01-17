#ifndef MEMORYSTRUCTURE_H
#define MEMORYSTRUCTURE_H

#include <stdbool.h>

typedef struct{

    unsigned int page;
    unsigned int LRUcounter;
    char referenceBit;
}memoryFrame;

typedef memoryFrame* memoryStructure;

int memory_lru(memoryStructure,unsigned int,unsigned int,bool*);
void memory_setCounter(memoryStructure,int,unsigned int);
int memory_secondChance(memoryStructure,unsigned int,unsigned int,bool*);
void memory_setReferenceBit(memoryStructure,int,unsigned int);
void memory_referenceBitRefresh(memoryStructure);

#endif