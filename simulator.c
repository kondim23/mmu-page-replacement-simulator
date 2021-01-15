#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

#define NUMBUCKETS 100

int main(int argc, char* argv[]) {

    int framesCount,q;
    FILE *bzipFile,*gccFile;
    unsigned int address,page,offset;
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

    /*Initializing Hashes*/

    bzipHash=pageHashInitialize(NUMBUCKETS);
    gccHash=pageHashInitialize(NUMBUCKETS);

    /*Initializing Main Memory-Frame Holders*/

    unsigned int memory[framesCount];
    for (int i=0 ; i<framesCount ; i++) memory[i]=-1;

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

            if (searchPage(page,bzipHash)) {
                printf("Error: insertion in bzip hash failed.\n");
                return 1;
            }
        }

        /*q traces for gcc*/

        for (int i=0 ; i<q ; i++) {
            if (!fscanf(gccFile, "%x %c", &address, &type)) {
                running=false;
                continue;
            }

            page=address>>12;
            offset=address<<20;

            if (searchPage(page,gccHash)) {
                printf("Error: insertion in gcc hash failed.\n");
                return 1;
            }
        }
    }
}