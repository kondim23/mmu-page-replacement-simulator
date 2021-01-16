#include <stdio.h>
#include "list.h"

pageHash pageHashInitialize(int numBuckets) {

    pageHash bzipHash = (pageHash) malloc(sizeof(listnode*)*numBuckets);
    for (int i=0 ; i<numBuckets ; i++) bzipHash[i]=NULL;
    return bzipHash;
}

int searchPage(unsigned int page, pageHash Hash, int numBuckets, unsigned int* memory) {

    int key,frameIndex;
    listnode *currentNode,*lastNode;

    key=hashFunction(page)%numBuckets;

    /*Alloc first node of list*/
    if (Hash[key]==NULL) {

        /*alloc new node*/
        Hash[key]=(listnode*)malloc(sizeof(listnode));
        Hash[key]->page=page;
        Hash[key]->nextblock=NULL;
        /*Find proper frame*/
        Hash[key]->frame=replacementAlgorithm(memory,page);
        return Hash[key]->frame;
    }

    currentNode=Hash[key];
    while (currentNode!=NULL) {

        if (currentNode->page==page) {

            /*probably does something here*/
            return currentNode->frame;
        }
        lastNode=currentNode;
        currentNode=currentNode->nextblock;
    }

    /*node doesnt exist*/
    /*alloc new node*/
    lastNode->nextblock=(listnode*)malloc(sizeof(listnode));
    lastNode->nextblock->page=page;
    lastNode->nextblock->nextblock=NULL;
    /*Find proper frame*/
    lastNode->nextblock->frame=replacementAlgorithm(memory,page);
    return lastNode->nextblock->frame;
}

int deletePage(unsigned int page, pageHash Hash, int numBuckets, unsigned int* memory) {

    int key;
    listnode *currentNode, *lastNode;

    key=hashFunction(page)%numBuckets;

    /*Checks for empty list*/
    if (Hash[key]==NULL) {
        printf("Error: page not exists in hash\n");
        return -1;
    }


    if (Hash[key]->page==page) {
        listnode *temp=Hash[key];
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

int pageHashDestroy(pageHash Hash, int numBuckets) {

    listnode *temp,*current;

    for (int i=0 ; i<numBuckets ; i++) {

        current=Hash[i];
        while (current!=NULL) {

            temp=current;
            current=current->nextblock;
            free(temp);
        }
    }

    free(Hash);
}