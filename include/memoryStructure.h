#ifndef MEMORYSTRUCTURE_H
#define MEMORYSTRUCTURE_H

#include "pageTable.h"

typedef struct{
    hashnode *page; /*pointer to a hashnode holding page of frame*/
    pageHash Hash;  /*hash that holds the page of current frame*/
    char dirtyBit;  /*dirty bit of frame indicating writing to disk*/
}memoryFrame;

typedef memoryFrame* memoryStructure;

/*LRU algorithm*/
int memory_lru(memoryStructure,hashnode**,pageHash*,char);

/*sets dirty bit properly according to mode given ('R' or 'W')*/
void memory_setDirtyBit(memoryStructure,int,char);

/*secondChance algorithm*/
int memory_secondChance(memoryStructure,hashnode**,pageHash*,char);

/*refresh all reference bits*/
void memory_referenceBitRefresh(memoryStructure);

/*return the count of (dirtybit==1) in memory*/
int memory_getActiveDirtyBitCount(memoryStructure );

#endif