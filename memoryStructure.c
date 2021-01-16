#include <stdio.h>
#include "memoryStructure.h"
#include "list.h"

extern int framesCount;

void lruSetCounter(memoryStructure memory,int frame,unsigned int count) {

    memory[frame].LRUcounter=count;
    return;
}

void setReferenceBit(memoryStructure memory, int frame, unsigned int value) {

    memory[frame].referenceBit=1;
    return;
}

unsigned int lru(memoryStructure memory,unsigned int page,unsigned int count,pageHash Hash, int numBuckets) {

    int minIndex=-1;
    unsigned int minCounter=-1;

    for (int i=0 ; i<framesCount ; i++) {

        if (memory[i].LRUcounter<minCounter || minCounter==-1) {
            minIndex=i;
            minCounter=memory[i].LRUcounter;
            if (memory[i].LRUcounter==0) break;
        }
    }

    if (memory[minIndex].LRUcounter!=0){
        if (deletePage(memory[minIndex].page,Hash,numBuckets)<0) {
            printf("Error in page removal from hash\n");
            return 1;
        }
    }
    memory[minIndex].page=page;
    memory[minIndex].LRUcounter=count;
    return minIndex;
}

unsigned int secondChance(memoryStructure memory,unsigned int page,unsigned int count, pageHash Hash,int numBuckets) {

    static int index=0;

    while (memory[index].referenceBit==1){
        memory[index++].referenceBit=0;
        if (index==framesCount) 
            index=0;
    }

    if (deletePage(memory[index].page,Hash,numBuckets)<0) {
        printf("Error in page removal from hash\n");
        return 1;
    }
    memory[index].page=page;
    memory[index].referenceBit=1;
    return index;
}