#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pageTable.h"
#include "memoryStructure.h"

int (*memory_replacementAlgorithm) (memoryStructure,unsigned int*,unsigned int,pageHash*);
void (*memory_setMemoryAttribute) (memoryStructure,int,unsigned int);

int numFrames,numBuckets=1000;

int main(int argc, char* argv[]) {

    int q,frame,memory_referenceBitTimer;
    FILE *bzipFile,*gccFile;
    unsigned int address,page,offset, counter=0, maxTracesCount, readCount=0, writeCount=0, pageFaultCount=0, bzipTracesCount=0, gccTracesCount=0;
    pageHash bzipHash, gccHash, Hash;
    hashnode* Node;
    bool running=true,gccUnfinished=true,bzipUnfinished=true,secondChance=false;
    char type;

    /*Checking for proper input.*/

    if (argc!=5 || (strcmp(argv[1],"lru") && strcmp(argv[1],"secondChance")) || atoi(argv[2])<1 || atoi(argv[3])<1) {
        printf("User did not provide proper arguments.\n");
        return 1;
    }

    numFrames=atoi(argv[2]);
    q=atoi(argv[3]);
    maxTracesCount=atoi(argv[4]);
    if (!strcmp(argv[1],"lru")){
        memory_replacementAlgorithm = &memory_lru;
        memory_setMemoryAttribute = &memory_setCounter;
    }
    else if(!strcmp(argv[1],"secondChance")){
        memory_replacementAlgorithm = &memory_secondChance;
        memory_setMemoryAttribute = &memory_setReferenceBit;
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
        memory[i].LRUcounter=0;
        memory[i].referenceBit=0;
    }
    /*Opening trace files*/

    bzipFile=fopen("bzip.trace","r");
    gccFile=fopen("gcc.trace","r");

    while (running) {

        /*q traces for bzip*/
        if (bzipUnfinished)
            for (int i=0 ; i<q ; i++) {

                if (maxTracesCount>0 && counter>=maxTracesCount) {
                    running=false;
                    break;
                }
                if (fscanf(bzipFile, "%x %c", &address, &type)==EOF) {
                    if (gccUnfinished==false) running=false;
                    bzipUnfinished=false;
                    break;
                }
                counter++;
                bzipTracesCount++;
                if (secondChance && counter%memory_referenceBitTimer==0) memory_referenceBitRefresh(memory);

                page=address>>12;
                offset=address<<20>>20;

                if ((frame=hash_searchPage(page,bzipHash,type,&Node))<0) {

                    readCount++;
                    pageFaultCount++;
                    Hash=bzipHash;
                    frame=(*memory_replacementAlgorithm)(memory,&page,counter,&Hash);
                    if (Hash!=NULL) if (hash_removePage(page,Hash)=='W') writeCount++;
                    hash_setFrame(Node,frame);
                }
                else (*memory_setMemoryAttribute)(memory,frame,counter);
                printf("Process: bzip\tTrace: %08x %c Physical address: %08x\n",address,type,(unsigned int)((unsigned int)frame*(unsigned int)4096+offset));
            }

        /*q traces for gcc*/
        if (gccUnfinished)
            for (int i=0 ; i<q ; i++) {

                if (maxTracesCount>0 && counter>=maxTracesCount) {
                    running=false;
                    break;
                }
                if (fscanf(gccFile, "%x %c", &address, &type)==EOF) {
                    if (bzipUnfinished==false) running=false;
                    gccUnfinished=false;
                    break;
                }
                counter++;
                gccTracesCount++;
                if (secondChance && counter%memory_referenceBitTimer==0) memory_referenceBitRefresh(memory);

                page=address>>12;
                offset=address<<20>>20;

                if ((frame=hash_searchPage(page,gccHash,type,&Node))<0) {

                    readCount++;
                    pageFaultCount++;
                    Hash=gccHash;
                    frame=(*memory_replacementAlgorithm)(memory,&page,counter,&Hash);
                    if (Hash!=NULL) if (hash_removePage(page,Hash)=='W') writeCount++;
                    hash_setFrame(Node,frame);
                }
                else (*memory_setMemoryAttribute)(memory,frame,counter);
                printf("Process: gcc\tTrace: %08x %c Physical address: %08x\n",address,type,(unsigned int)((unsigned int)frame*(unsigned int)4096+offset));
            }
    }

    writeCount+= hash_destroy(gccHash) + hash_destroy(bzipHash);

    printf("Pages read from disk: %d\nPages written to disk: %d\nPage Faults: %d\nBzip Traces used: %d\nGcc  Traces used: %d\nFrames Count: %d\n",\
        readCount,writeCount,pageFaultCount,bzipTracesCount,gccTracesCount,numFrames);
    return 0;
}