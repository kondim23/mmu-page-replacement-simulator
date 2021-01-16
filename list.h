#ifndef LIST_H
#define LIST_H

typedef struct listnode{
    unsigned int page;
    int frame;
    struct listnode* nextblock;
}listnode;

typedef listnode** pageHash;

pageHash pageHashInitialize(int);
int searchPage(unsigned int, pageHash, int, unsigned int*);
int deletePage(unsigned int, pageHash, int, unsigned int*);
int pageHashDestroy(pageHash, int);

#endif