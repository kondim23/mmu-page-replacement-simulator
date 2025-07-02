#include <stdio.h>
#include <stdbool.h>
#include "memoryStructure.h"
#include "pageTable.h"

extern int numFrames;
extern unsigned int writeCount;

void memory_setDirtyBit(memoryStructure memory,int frame,char mode) {
    /*Set the dirty bit of page properly*/
    if (memory[frame].dirtyBit!=1 && mode=='W') memory[frame].dirtyBit=1;
    return;
}

int memory_lru(memoryStructure memory,hashnode **page,pageHash* Hash,char mode) {
    /*LRU algorithm*/

    int minIndex=-1;
    unsigned int minCounter=0,LRUcounter;
    hashnode* tempPage;
    pageHash tempHash;


    for (int i=0 ; i<numFrames ; i++) {

        /*Find the minimum LRUcount for every existing page*/
        LRUcounter=hash_getLRUcounter(memory[i].page);
        if (LRUcounter<=minCounter || minIndex==-1) {
            minIndex=i;
            minCounter=LRUcounter;

            /*LRUcounter==0 -> empty frame*/
            if (LRUcounter==0) break;
        }
    }

    /*If the old frame held a dirty bit==1 write to disk (writeCount++)*/
    if (memory[minIndex].dirtyBit==1) writeCount++;

    /*Exchange the page given and the page selected by LRU algorithm*/
    tempPage=memory[minIndex].page;
    memory[minIndex].page=*page;
    *page=tempPage;

    /*Exchange the hash given and the hash selected by LRU algorithm*/
    tempHash=memory[minIndex].Hash;
    memory[minIndex].Hash=*Hash;
    *Hash=tempHash;

    /*Properly set the dirty bit of new frame depending on mode*/
    if (mode=='W') memory[minIndex].dirtyBit=1;
    else memory[minIndex].dirtyBit=0;
    
    return minIndex;
}

int memory_secondChance(memoryStructure memory,hashnode **page,pageHash* Hash,char mode) {

    static int index=-1;
    hashnode* tempPage;
    pageHash tempHash;

    /*index moves through [0,numFrames]*/
    if (++index==numFrames) 
        index=0;

    /*get the first frame with reference bit==0*/
    while (hash_getReferenceBit(memory[index].page)==1){

        /*reference bit==1 -> reinitialize it to 0*/
        hash_setReferenceBitZero(memory[index].page);
        index++;
        if (index==numFrames) 
            index=0;
    }

    /*If the old frame held a dirty bit==1 write to disk (writeCount++)*/
    if (memory[index].dirtyBit==1) writeCount++;
    
    /*Exchange the page given and the page selected by secondChance algorithm*/
    tempPage=memory[index].page;
    memory[index].page=*page;
    *page=tempPage;

    /*Exchange the hash given and the hash selected by secondChance algorithm*/
    tempHash=memory[index].Hash;
    memory[index].Hash=*Hash;
    *Hash=tempHash;
    
    /*Properly set the dirty bit of new frame depending on mode*/
    if (mode=='W') memory[index].dirtyBit=1;
    else memory[index].dirtyBit=0;

    return index;
}

void memory_referenceBitRefresh(memoryStructure memory) {
    /*Set all reference bits to 0*/
    for (int i=0 ; i<numFrames ; i++) hash_setReferenceBitZero(memory[i].page);
    return;
}

int memory_getActiveDirtyBitCount(memoryStructure memory) {
    /*Return the count of (dirtybit==1) in memory*/
    int count=0;

    for (int i=0 ; i<numFrames ; i++) if (memory[i].dirtyBit==1) count++;
    return count;
}