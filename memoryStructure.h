#ifndef MEMORY_H
#define MEMORY_H

typedef struct{

    unsigned int page;
    unsigned int LRUcounter;
    char referenceBit;
}memoryFrame;

typedef memoryFrame* memoryStructure;

unsigned int lru(memoryStructure,unsigned int,unsigned int,pageHash,int);
void lruSetCounter(memoryStructure,int,unsigned int);
unsigned int secondChance(memoryStructure,unsigned int,unsigned int,pageHash,int);
void setReferenceBit(memoryStructure,int,unsigned int);

#endif