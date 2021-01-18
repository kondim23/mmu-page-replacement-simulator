#ifndef PAGETABLE_H
#define PAGETABLE_H

#include "memoryStructure.h"

typedef struct hashnode{
    unsigned int page;
    int frame;
    char type;
    struct hashnode* nextblock;
}hashnode;

typedef hashnode** pageHash;

pageHash hash_Initialize();
int hash_searchPage(unsigned int, pageHash,char, hashnode**);
void hash_setFrame(hashnode*, int);
char hash_removePage(unsigned int, pageHash);
void hash_destroy(pageHash);
int hash_function(unsigned int);

#endif