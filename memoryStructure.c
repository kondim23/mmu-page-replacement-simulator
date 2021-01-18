#include <stdio.h>
#include <stdbool.h>
#include "memoryStructure.h"
#include "pageTable.h"

extern int numFrames;
extern unsigned int writeCount;

void memory_setDirtyBit(memoryStructure memory,int frame,char type) {

    if (memory[frame].dirtyBit!=1 && type=='W') memory[frame].dirtyBit=1;
    return;
}

int memory_lru(memoryStructure memory,hashnode **page,unsigned int count,pageHash* Hash,char type) {

    int minIndex=-1;
    unsigned int minCounter=0,LRUcounter;
    hashnode* tempPage;
    pageHash tempHash;


    for (int i=0 ; i<numFrames ; i++) {

        LRUcounter=hash_getLRUcounter(memory[i].page);

        if (LRUcounter<=minCounter || minIndex==-1) {
            minIndex=i;
            minCounter=LRUcounter;
            if (LRUcounter==0) break;
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

    if (memory[minIndex].dirtyBit==1) writeCount++;

    tempPage=memory[minIndex].page;
    memory[minIndex].page=*page;
    *page=tempPage;

    tempHash=memory[minIndex].Hash;
    memory[minIndex].Hash=*Hash;
    *Hash=tempHash;

    if (type=='W') memory[minIndex].dirtyBit=1;
    else memory[minIndex].dirtyBit=0;
    
    // memory[minIndex].page->LRUcounter=count;
    return minIndex;
}

int memory_secondChance(memoryStructure memory,hashnode **page,unsigned int count,pageHash* Hash,char type) {

    static int index=-1;
    hashnode* tempPage;
    pageHash tempHash;

    if (++index==numFrames) 
        index=0;

    while (hash_getReferenceBit(memory[index].page)==1){
        hash_referenceBitRefresh(memory[index].page);
        index++;
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
    
    return index;
}

void memory_referenceBitRefresh(memoryStructure memory) {

    for (int i=0 ; i<numFrames ; i++) hash_referenceBitRefresh(memory[i].page);
    return;
}

int memory_getActiveDirtyBitCount(memoryStructure memory) {

    int count=0;

    for (int i=0 ; i<numFrames ; i++) if (memory[i].dirtyBit==1) count++;
    return count;
}