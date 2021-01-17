#include <stdio.h>
#include <stdbool.h>
#include "memoryStructure.h"
#include "pageTable.h"

extern int numFrames;

void memory_setCounter(memoryStructure memory,int frame,unsigned int count) {

    memory[frame].LRUcounter=count;
    return;
}

void memory_setReferenceBit(memoryStructure memory, int frame, unsigned int value) {

    memory[frame].referenceBit=1;
    return;
}

int memory_lru(memoryStructure memory,unsigned int page,unsigned int count,bool* replacement) {

    int minIndex=-1;
    unsigned int minCounter=0;

    for (int i=0 ; i<numFrames ; i++) {

        if (memory[i].LRUcounter<=minCounter || minIndex==-1) {
            minIndex=i;
            minCounter=memory[i].LRUcounter;
            if (memory[i].LRUcounter==0) break;
        }
    }

    if (memory[minIndex].LRUcounter!=0){
        // if (deletePage(memory[minIndex].page,Hash,numBuckets)<0) {
        //     printf("Error in page removal from hash\n");
        //     return 1;
        // }
        *replacement=true;
    }
    else *replacement=false;
    memory[minIndex].page=page;
    memory[minIndex].LRUcounter=count;
    return minIndex;
}

int memory_secondChance(memoryStructure memory,unsigned int page,unsigned int count,bool* replacement) {

    static int index=-1;

    if (++index==numFrames) 
        index=0;

    while (memory[index].referenceBit==1){
        memory[index++].referenceBit=0;
        if (index==numFrames) 
            index=0;
    }

    if (memory[index].page!=0) *replacement=true;
    else *replacement=false;
    
    memory[index].page=page;
    memory[index].referenceBit=1;
    return index;
}

void memory_referenceBitRefresh(memoryStructure memory) {

    for (int i=0 ; i<numFrames ; i++) memory[i].referenceBit=0;
    return;
}