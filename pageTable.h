#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef struct hashnode{
    unsigned int page;
    int frame;
    unsigned int LRUcounter;
    char referenceBit;
    struct hashnode* nextblock;
}hashnode;

typedef hashnode** pageHash;

pageHash hash_Initialize();
int hash_searchPage(unsigned int, pageHash,char, hashnode**,unsigned int);
void hash_setFrame(hashnode*, int);
int hash_removePage(unsigned int, pageHash);
int hash_destroy(pageHash);
int hash_function(unsigned int);
void hash_referenceBitRefresh(hashnode* page);
int hash_getReferenceBit(hashnode* page);
unsigned int hash_getLRUcounter(hashnode* page);

#endif