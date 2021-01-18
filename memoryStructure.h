#ifndef MEMORYSTRUCTURE_H
#define MEMORYSTRUCTURE_H

#include "pageTable.h"

typedef struct{
    hashnode *page;
    char dirtyBit;
    pageHash Hash;
}memoryFrame;

typedef memoryFrame* memoryStructure;

int memory_lru(memoryStructure,hashnode**,unsigned int,pageHash*,char);
void memory_setDirtyBit(memoryStructure,int,char);
int memory_secondChance(memoryStructure,hashnode**,unsigned int,pageHash*,char);
void memory_referenceBitRefresh(memoryStructure);
int memory_getActiveDirtyBitCount(memoryStructure );

#endif