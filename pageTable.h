#ifndef PAGETABLE_H
#define PAGETABLE_H

typedef struct hashnode{
    unsigned int page;
    int frame;
    unsigned int LRUcounter;    /*used in LRU algorithm*/
    char referenceBit;          /*used in secondChance algorithm*/
    struct hashnode* nextblock;
}hashnode;

typedef hashnode** pageHash;

/*Return an allocated-initialized hash*/
pageHash hash_Initialize();

/*Add the page in hash if it doesnt exist*/
int hash_searchPage(unsigned int, pageHash,char, hashnode**,unsigned int);

/*Set frame of page equal to frame given*/
void hash_setFrame(hashnode*, int);

/*Remove page from hash*/
int hash_removePage(unsigned int, pageHash);

/*free given hash*/
int hash_destroy(pageHash);

/*modulo hash function*/
int hash_function(unsigned int);

/*set the reference bit of given page to 0*/
void hash_setReferenceBitZero(hashnode* );

/*get reference bit of given page*/
int hash_getReferenceBit(hashnode* );

/*get LRU counter of given page*/
unsigned int hash_getLRUcounter(hashnode* );

/*get pageId of given page*/
unsigned int hash_getPage(hashnode *);

#endif