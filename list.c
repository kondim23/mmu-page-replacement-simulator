#include <stdio.h>
#include "list.h"

pageHash pageHashInitialize(int numBuckets) {

    pageHash bzipHash = (pageHash) malloc(sizeof(listnode*)*numBuckets);
    for (int i=0 ; i<numBuckets ; i++) bzipHash[i]=NULL;
    return bzipHash;
}

int searchPage(unsigned int page, pageHash Hash, int numBuckets) {

    int key;
    listnode *currentNode,*lastNode;

    key=hashFunction(page)%numBuckets;

    /*Alloc first node of list*/
    if (Hash[key]==NULL) {

        /*alloc new node*/
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
}