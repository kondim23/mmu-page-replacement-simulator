#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pageTable.h"
#include "memoryStructure.h"

/*Holds lru or secondChance functions*/
int (*memory_replacementAlgorithm) (memoryStructure,hashnode**,pageHash*,char);

int numFrames,numBuckets=100;
unsigned int writeCount=0;

int main(int argc, char* argv[]) {

    int q,frame,memory_referenceBitTimer;
    FILE *bzipFile,*gccFile;
    unsigned int address,pageId,offset, counter=0, maxTracesCount, readCount=0,\
     pageFaultCount=0, bzipTracesCount=0, gccTracesCount=0;
    pageHash bzipHash, gccHash, hashPtr;
    hashnode *pagePtr,*insertedPage;
    bool running=true,gccUnfinished=true,bzipUnfinished=true,secondChance=false;
    char mode;


    /*Checking for proper input.*/

    if (argc!=5 || (strcmp(argv[1],"lru") && strcmp(argv[1],"secondChance")) ||\
     atoi(argv[2])<1 || atoi(argv[3])<1) {
        printf("User did not provide proper arguments.\n");
        return 1;
    }

    numFrames=atoi(argv[2]);
    q=atoi(argv[3]);
    maxTracesCount=atoi(argv[4]);
    if (!strcmp(argv[1],"lru")){
        memory_replacementAlgorithm = &memory_lru;
    }
    else if(!strcmp(argv[1],"secondChance")){
        memory_replacementAlgorithm = &memory_secondChance;
        memory_referenceBitTimer = numFrames/3;
        secondChance=true;
    }


    /*Initializing Hashes*/

    bzipHash=hash_Initialize();
    gccHash=hash_Initialize();


    /*Initializing Main Memory-Frame Holders*/

    memoryFrame memory[numFrames];
    for (int i=0 ; i<numFrames ; i++) {
        memory[i].page=0;
        memory[i].Hash=NULL;
        memory[i].dirtyBit=0;
    }


    /*Opening trace files*/

    bzipFile=fopen("bzip.trace","r");
    gccFile=fopen("gcc.trace","r");

    while (running) {

        /*q traces for bzip*/
        if (bzipUnfinished)
            for (int i=0 ; i<q ; i++) {

                /*Limit of traces count by user reached*/
                if (maxTracesCount>0 && counter>=maxTracesCount) {
                    running=false;
                    break;
                }

                /*Receiving a trace from bzip trace file */
                if (fscanf(bzipFile, "%x %c", &address, &mode)==EOF) {
                    if (gccUnfinished==false) running=false;
                    bzipUnfinished=false;
                    break;
                }
                counter++;
                bzipTracesCount++;

                /*Set all Reference bits to 0 every numTraces/3 traces in case of secondChance*/
                if (secondChance && counter%memory_referenceBitTimer==0) 
                    memory_referenceBitRefresh(memory);

                /*offset is defined by the last 12 bits of address (2^12=4096)*/
                /*pageId is defined by the remaining bits (20 leftmost bits)*/
                pageId=address>>12;
                offset=address<<20>>20;

                /*Add the page in bzip hash if it doesnt exist*/
                /*pagePtr is a ptr to the page-node in hash*/
                if ((frame=hash_searchPage(pageId,bzipHash,mode,&pagePtr,counter))<0) {

                    /*frame==-1 --> page doesnt exist in hash*/
                    /*we read the page from disk (readCount++)*/
                    /*as we received page fault (pageFaultCount++)*/
                    readCount++;
                    pageFaultCount++;

                    hashPtr=bzipHash;
                    insertedPage=pagePtr;

                    /*Find a proper position in memory for current page via lru or secondChance*/
                    
                    /*before memory_replacementAlgorithm():*/
                    /*pagePtr==pointer to current page, hashPtr==pointer to bzip hash*/
                    frame=(*memory_replacementAlgorithm)(memory,&pagePtr,&hashPtr,mode);

                    /*after memory_replacementAlgorithm():*/
                    /*pagePtr==page to remove, hashPtr==hash that holds page to remove*/
                    
                    /*If a page existed on the used frame remove it*/
                    if (hashPtr!=NULL) hash_removePage(hash_getPage(pagePtr),hashPtr);
                    hash_setFrame(insertedPage,frame);
                }
                /*frame>0 --> page exists in hash*/
                /*Set dirty bit if needed*/
                else memory_setDirtyBit(memory,frame,mode);

                printf("Process: bzip\tTrace: %08x %c Physical address: %08x\n",address,mode,\
                (unsigned int)((unsigned int)frame*(unsigned int)4096+offset));
            }

        /*q traces for gcc*/
        if (gccUnfinished)
            for (int i=0 ; i<q ; i++) {

                /*Limit of traces count by user reached*/
                if (maxTracesCount>0 && counter>=maxTracesCount) {
                    running=false;
                    break;
                }

                /*Receiving a trace from gcc trace file */
                if (fscanf(gccFile, "%x %c", &address, &mode)==EOF) {
                    if (bzipUnfinished==false) running=false;
                    gccUnfinished=false;
                    break;
                }
                counter++;
                gccTracesCount++;

                /*Set all Reference bits to 0 every numTraces/3 traces in case of secondChance*/
                if (secondChance && counter%memory_referenceBitTimer==0) 
                    memory_referenceBitRefresh(memory);

                /*offset is defined by the last 12 bits of address (2^12=4096)*/
                /*pageId is defined by the remaining bits (20 leftmost bits)*/
                pageId=address>>12;
                offset=address<<20>>20;

                /*Add the page in gcc hash if it doesnt exist*/
                /*pagePtr is a ptr to the page-node in hash*/
                if ((frame=hash_searchPage(pageId,gccHash,mode,&pagePtr,counter))<0) {

                    /*frame==-1 --> page doesnt exist in hash*/
                    /*we read the page from disk (readCount++)*/
                    /*as we received page fault (pageFaultCount++)*/
                    readCount++;
                    pageFaultCount++;

                    hashPtr=gccHash;
                    insertedPage=pagePtr;

                    /*Find a proper position in memory for current page via lru or secondChance*/
                    
                    /*before memory_replacementAlgorithm():*/
                    /*pagePtr==pointer to current page, hashPtr==pointer to gcc hash*/
                    frame=(*memory_replacementAlgorithm)(memory,&pagePtr,&hashPtr,mode);
                    /*after memory_replacementAlgorithm():*/
                    /*pagePtr==page to remove, hashPtr==hash that holds page to remove*/
                    
                    /*If a page existed on the used frame remove it*/
                    if (hashPtr!=NULL) hash_removePage(hash_getPage(pagePtr),hashPtr);
                    hash_setFrame(insertedPage,frame);
                }
                /*frame>0 --> page exists in hash*/
                /*Set dirty bit if needed*/
                else memory_setDirtyBit(memory,frame,mode);

                printf("Process: gcc\tTrace: %08x %c Physical address: %08x\n",address,mode,\
                (unsigned int)((unsigned int)frame*(unsigned int)4096+offset));
            }
    }

    /*Write the remaining pages in disk*/
    writeCount+= memory_getActiveDirtyBitCount(memory);

    /*Destroy hashes - close files*/
    hash_destroy(gccHash);
    hash_destroy(bzipHash);
    fclose(gccFile);
    fclose(bzipFile);

    printf("Pages read from disk: %d\nPages written to disk: %d\nPage Faults: \
    %d\nBzip Traces used: %d\nGcc  Traces used: %d\nFrames Count: %d\n",\
        readCount,writeCount,pageFaultCount,bzipTracesCount,gccTracesCount,numFrames);

    return 0;
}