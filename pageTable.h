#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "memoryStructure.h"

typedef struct hashnode{
    unsigned int page;
    int frame;
    struct hashnode* nextblock;
}hashnode;

typedef hashnode** pageHash;

pageHash hash_Initialize();
int hash_searchPage(unsigned int, pageHash, hashnode**);
void hash_setFrame(hashnode*, int);
int hash_removePage(unsigned int, pageHash);
int hash_destroy(pageHash);
int hash_function(unsigned int);

#endif