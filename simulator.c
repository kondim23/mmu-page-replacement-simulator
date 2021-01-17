#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "pageTable.h"
#include "memoryStructure.h"

int (*memory_replacementAlgorithm) (memoryStructure,unsigned int,unsigned int,bool*);
void (*memory_setMemoryAttribute) (memoryStructure,int,unsigned int);

int numFrames,numBuckets=100;

int main(int argc, char* argv[]) {

    int q;
    FILE *bzipFile,*gccFile;
    unsigned int address,page,offset,frame, counter=0;
    pageHash bzipHash, gccHash;
    hashnode* Node;
    bool running,Replacement;
    char type;

    /*Checking for proper input.*/

    if (argc!=4 || (strcmp(argv[1],"lru") && strcmp(argv[1],"secondChance")) || atoi(argv[2])<1 || atoi(argv[3])<1) {
        printf("Please provide proper arguments.\n");
        return 1;
    }

    numFrames=atoi(argv[2]);
    q=atoi(argv[3]);
    if (!strcmp(argv[1],"lru")){
        memory_replacementAlgorithm = &memory_lru;
        memory_setMemoryAttribute = &memory_setCounter;
    }
    else if(!strcmp(argv[1],"secondChance")){
        memory_replacementAlgorithm = &memory_secondChance;
        memory_setMemoryAttribute = &memory_setReferenceBit;
    }

    /*Initializing Hashes*/

    bzipHash=hash_Initialize();
    gccHash=hash_Initialize();

    /*Initializing Main Memory-Frame Holders*/

    memoryFrame memory[numFrames];
    for (int i=0 ; i<numFrames ; i++) {
        memory[i].page=0;
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
            counter++;

            page=address>>12;
            offset=address<<20;

            if ((frame=hash_searchPage(page,bzipHash,&Node))<0) {

                frame=(*memory_replacementAlgorithm)(memory,page,counter,&Replacement);
                if (Replacement) hash_removePage(page,bzipHash);
                hash_setFrame(Node,frame);
            }
            else (*memory_setMemoryAttribute)(memory,frame,counter);
            printf("Physical address: %8x\n",(unsigned int)(frame*4096+offset));
        }

        /*q traces for gcc*/

        for (int i=0 ; i<q ; i++) {
            if (!fscanf(gccFile, "%x %c", &address, &type)) {
                running=false;
                continue;
            }
            counter++;

            page=address>>12;
            offset=address<<20;

            if ((frame=hash_searchPage(page,gccHash,&Node))<0) {

                frame=(*memory_replacementAlgorithm)(memory,page,counter,&Replacement);
                if (Replacement) hash_removePage(page,gccHash);
                hash_setFrame(Node,frame);
            }
            else (*memory_setMemoryAttribute)(memory,frame,counter);
            printf("Physical address: %8x\n",(unsigned int)(frame*4096+offset));
        }
    }

    hash_destroy(gccHash);
    hash_destroy(bzipHash);
    return 0;
}