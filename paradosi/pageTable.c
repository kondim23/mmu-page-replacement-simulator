#include <stdio.h>
#include <stdlib.h>
#include "pageTable.h"
#include "memoryStructure.h"

extern int numBuckets;

pageHash hash_Initialize() {

    /*Allocate a hash of hashnode pointers*/
    pageHash Hash = (pageHash) malloc(sizeof(hashnode*)*numBuckets);
    for (int i=0 ; i<numBuckets ; i++) Hash[i]=NULL;
    return Hash;
}

int hash_function(unsigned int page) {
    /*Modulo hash funtion*/
    return page%numBuckets;
}

int hash_searchPage(unsigned int page, pageHash Hash,char type, hashnode** node,unsigned int counter) {

    int key;
    hashnode *currentNode,*lastNode;
    
    key=hash_function(page)%numBuckets;

    /*In case of empty bucket*/
    if (Hash[key]==NULL) {

        /*allocate new node*/
        Hash[key]=(hashnode*)malloc(sizeof(hashnode));

        Hash[key]->page=page;
        Hash[key]->LRUcounter=counter;
        Hash[key]->referenceBit=1;
        Hash[key]->nextblock=NULL;

        /*return a pointer to this hashnode*/
        *node=Hash[key];

        /*Page didnt exist in hash -> returns "frame"=-1*/
        return -1;
    }

    /*Navigate in bucket*/
    currentNode=Hash[key];
    while (currentNode!=NULL) {

        /*Desired page found*/
        if (currentNode->page==page) {

            /*Updating counter, referenceBit for LRU, secondChance*/
            currentNode->LRUcounter=counter;
            currentNode->referenceBit=1;
            /*Page existed -> return frame*/
            return currentNode->frame;
        }
        lastNode=currentNode;
        currentNode=currentNode->nextblock;
    }

    /*Page not in bucket -> Allocate a new hashnode for page*/
    lastNode->nextblock=(hashnode*)malloc(sizeof(hashnode));

    lastNode->nextblock->page=page;
    lastNode->nextblock->LRUcounter=counter;
    lastNode->nextblock->referenceBit=1;
    lastNode->nextblock->nextblock=NULL;

    /*return a pointer to this hashnode*/
    *node=lastNode->nextblock;

    /*Page didnt exist in hash -> returns "frame"=-1*/
    return -1;
}

void hash_setFrame(hashnode* page, int frame) {

    /*Set frame of page equal to frame given*/
    page->frame=frame;
    return;
}

int hash_removePage(unsigned int page, pageHash Hash) {

    int key;
    char type;
    hashnode *currentNode, *lastNode, *temp;

    key=hash_function(page)%numBuckets;

    /*Checks for empty list*/
    if (Hash[key]==NULL) {
        printf("Error: page not exists in hash\n");
        return -1;
    }

    /*Page is the first hashnode of bucket*/
    if (Hash[key]->page==page) {
        temp=Hash[key];
        Hash[key]=Hash[key]->nextblock;
        free(temp);
        return 0;
    }

    /*We navigate in bucket*/
    currentNode=Hash[key]->nextblock;
    lastNode=Hash[key];
    while (currentNode!=NULL) {

        if (currentNode->page==page) {

            /*Desired page found*/
            lastNode->nextblock=currentNode->nextblock;
            free(currentNode);
            return 0;
        }
        lastNode=currentNode;
        currentNode=currentNode->nextblock;
    }

    printf("Error: page not found in hash\n");
    return -1;
}

int hash_destroy(pageHash Hash) {

    hashnode *temp,*current;
    int writec=0;

    for (int i=0 ; i<numBuckets ; i++) {

        current=Hash[i];
        /*free all hashnodes of bucket i*/
        while (current!=NULL) {

            temp=current;
            current=current->nextblock;
            free(temp);
        }
    }
    /*free hash*/
    free(Hash);
    return writec;
}

void hash_setReferenceBitZero(hashnode* page) {
    /*Sets reference bit of page to 0*/
    if (page!=NULL) page->referenceBit=0;
    return;
}

int hash_getReferenceBit(hashnode* page) {
    /*return reference bit of page*/
    if (page!=NULL) return page->referenceBit;
    else return 0;
}

unsigned int hash_getLRUcounter(hashnode* page) {
    /*return LRUCounter of page*/
    if (page!=NULL) return page->LRUcounter;
    else return 0;
}

unsigned int hash_getPage(hashnode *pageptr) {
    /*return pageId*/
    return pageptr->page;
}