#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "memoryStructure.h"

#define NUMBUCKETS 100

unsigned int (*replacementAlgorithm) (memoryStructure,unsigned int,unsigned int,pageHash,int);
void (*setMemoryAttribute) (memoryStructure,int,unsigned int);

int framesCount;

int main(int argc, char* argv[]) {

    int q;
    FILE *bzipFile,*gccFile;
    unsigned int address,page,offset,frame;
    pageHash bzipHash, gccHash;
    bool running;
    char type;

    /*Checking for proper input.*/

    if (argc!=4 || (strcmp(argv[1],"lru") && strcmp(argv[1],"secondChance")) || atoi(argv[2])<1 || atoi(argv[3])<1) {
        printf("Please provide proper arguments.\n");
        return 1;
    }

    framesCount=atoi(argv[2]);
    q=atoi(argv[3]);
    if (!strcmp(argv[1],"lru")){
        replacementAlgorithm = &lru;
        setMemoryAttribute = &lruSetCounter;
    }
    else if(!strcmp(argv[1],"secondChance")){
        replacementAlgorithm = &secondChance;
        setMemoryAttribute = &setReferenceBit;
    }

    /*Initializing Hashes*/

    bzipHash=pageHashInitialize(NUMBUCKETS);
    gccHash=pageHashInitialize(NUMBUCKETS);

    /*Initializing Main Memory-Frame Holders*/

    memoryFrame memory[framesCount];
    for (int i=0 ; i<framesCount ; i++) {
        memory[i].LRUcounter=0;
        memory[i].referenceBit=0;
    }
    /*Opening trace files*/

    bzipFile=fopen("bzip.trace","r");
    gccFile=fopen("gcc.trace","r");

    running=true;
    while (running) {

        /*q traces for bzip*/

        for (int i=0 ; i<q ; i++) {

            if (!fscanf(bzipFile, "%x %c", &address, &type)) {
                running=false;
                continue;
            }

            page=address>>12;
            offset=address<<20;

            frame=searchPage(page,bzipHash,NUMBUCKETS,memory);
            printf("Physical address: %8x\n",(unsigned int)(frame*4096+offset));
        }

        /*q traces for gcc*/

        for (int i=0 ; i<q ; i++) {
            if (!fscanf(gccFile, "%x %c", &address, &type)) {
                running=false;
                continue;
            }

            page=address>>12;
            offset=address<<20;

            frame=searchPage(page,gccHash,NUMBUCKETS,memory);
            printf("Physical address: %8x\n",(unsigned int)(frame*4096+offset));
        }
    }

    pageHashDestroy(gccHash,NUMBUCKETS);
    pageHashDestroy(bzipHash,NUMBUCKETS);
}