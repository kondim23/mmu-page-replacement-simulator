#ifndef LIST_H
#define LIST_H

#include "memoryStructure.h"

typedef struct listnode{
    unsigned int page;
    int frame;
    struct listnode* nextblock;
}listnode;

typedef listnode** pageHash;

pageHash pageHashInitialize(int);
int searchPage(unsigned int, pageHash, int, listnode**);
void setHashFrame(listnode*, int);
int deletePage(unsigned int, pageHash, int);
int pageHashDestroy(pageHash, int);
int hashFunction(unsigned int);

#endif