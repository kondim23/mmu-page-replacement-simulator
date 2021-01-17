#include <stdio.h>
#include <stdlib.h>
#include "pageTable.h"
#include "memoryStructure.h"

extern int numBuckets;

// extern unsigned int (*replacementAlgorithm) (memoryStructure,unsigned int,unsigned int,pageHash,int);
// extern void (*setMemoryAttribute) (memoryStructure,int,unsigned int); 

pageHash hash_Initialize() {

    pageHash Hash = (pageHash) malloc(sizeof(hashnode*)*numBuckets);
    for (int i=0 ; i<numBuckets ; i++) Hash[i]=NULL;
    return Hash;
}

int hash_function(unsigned int page) {
    return 1;
}

int hash_searchPage(unsigned int page, pageHash Hash,hashnode** node) {

    int key;
    hashnode *currentNode,*lastNode;
    
    key=hashFunction(page)%numBuckets;

    /*Alloc first node of list*/
    if (Hash[key]==NULL) {

        /*alloc new node*/
        Hash[key]=(hashnode*)malloc(sizeof(hashnode));
        Hash[key]->page=page;
        Hash[key]->nextblock=NULL;
        /*Find proper frame*/
        // Hash[key]->frame=(*replacementAlgorithm)(memory,page,counter,Hash,numBuckets);
        // return Hash[key]->frame;
    
        *node=Hash[key];
        return -1;
    }

    currentNode=Hash[key];
    while (currentNode!=NULL) {

        if (currentNode->page==page) {

            /*probably does something here*/
            // (*setMemoryAttribute)(memory,currentNode->frame,counter);
            return currentNode->frame;
        }
        lastNode=currentNode;
        currentNode=currentNode->nextblock;
    }

    /*node doesnt exist*/
    /*alloc new node*/
    lastNode->nextblock=(hashnode*)malloc(sizeof(hashnode));
    lastNode->nextblock->page=page;
    lastNode->nextblock->nextblock=NULL;
    /*Find proper frame*/
    // lastNode->nextblock->frame=(*replacementAlgorithm)(memory,page,counter,Hash,numBuckets);
    // return lastNode->nextblock->frame;

    *node=lastNode->nextblock;
    return -1;
}

void hash_setFrame(hashnode* node, int frame) {

    node->frame=frame;
    return;
}

int hash_removePage(unsigned int page, pageHash Hash) {

    int key;
    hashnode *currentNode, *lastNode;

    key=hashFunction(page)%numBuckets;

    /*Checks for empty list*/
    if (Hash[key]==NULL) {
        printf("Error: page not exists in hash\n");
        return -1;
    }


    if (Hash[key]->page==page) {
        hashnode *temp=Hash[key];
        Hash[key]=Hash[key]->nextblock;
        free(temp);
        return 0;
    }

    currentNode=Hash[key]->nextblock;
    lastNode=Hash[key];
    while (currentNode!=NULL) {

        if (currentNode->page==page) {

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

void hash_destroy(pageHash Hash) {

    hashnode *temp,*current;

    for (int i=0 ; i<numBuckets ; i++) {

        current=Hash[i];
        while (current!=NULL) {

            temp=current;
            current=current->nextblock;
            free(temp);
        }
    }
    free(Hash);
    return;
}