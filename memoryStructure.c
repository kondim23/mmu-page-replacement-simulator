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

int memory_lru(memoryStructure memory,unsigned int *page,unsigned int count,pageHash* Hash) {

    int minIndex=-1;
    unsigned int minCounter=0,tempPage;
    pageHash tempHash;


    for (int i=0 ; i<numFrames ; i++) {

        if (memory[i].LRUcounter<=minCounter || minIndex==-1) {
            minIndex=i;
            minCounter=memory[i].LRUcounter;
            if (memory[i].LRUcounter==0) break;
        }
    }

    // if (memory[minIndex].LRUcounter!=0){
    //     // if (deletePage(memory[minIndex].page,Hash,numBuckets)<0) {
    //     //     printf("Error in page removal from hash\n");
    //     //     return 1;
    //     // }
    //     *Hash=memory[minIndex].Hash;
    // }
    // else *Hash=NULL;

    tempPage=memory[minIndex].page;
    memory[minIndex].page=*page;
    *page=tempPage;

    tempHash=memory[minIndex].Hash;
    memory[minIndex].Hash=*Hash;
    *Hash=tempHash;

    memory[minIndex].LRUcounter=count;
    return minIndex;
}

int memory_secondChance(memoryStructure memory,unsigned int *page,unsigned int count,pageHash* Hash) {

    static int index=-1;
    unsigned int tempPage;
    pageHash tempHash;

    if (++index==numFrames) 
        index=0;

    while (memory[index].referenceBit==1){
        memory[index++].referenceBit=0;
        if (index==numFrames) 
            index=0;
    }

    // if (memory[index].page!=0) *Hash=memory[index].Hash;
    // else *Hash=NULL;
    
    tempPage=memory[index].page;
    memory[index].page=*page;
    *page=tempPage;

    tempHash=memory[index].Hash;
    memory[index].Hash=*Hash;
    *Hash=tempHash;
    
    memory[index].referenceBit=1;
    return index;
}

void memory_referenceBitRefresh(memoryStructure memory) {

    for (int i=0 ; i<numFrames ; i++) memory[i].referenceBit=0;
    return;
}