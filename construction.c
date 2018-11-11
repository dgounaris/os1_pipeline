#include "construction.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include "semconfig.h"
#include <math.h>

void constructFirst(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from construction component 1\n");
    struct sembuf semDown = {3, -1, 0};
    struct sembuf semUp = {3, 1, 0};
    printf("Construction 1 pos: %p Next: %p\n", myBufferPos, nextBufferPos);
    int i=0;
    for (i=0;i<elements;i++) {
        int msPause = rand() % 50;
        struct timespec ts;
        ts.tv_sec = msPause / 1000;
        ts.tv_nsec = (msPause % 1000) * 1000000;
        nanosleep(&ts, NULL);
        semDown.sem_num = 0;
        semop(semId, &semDown, 1); //down first availability
        myBufferPos->id = rand() % 10000;
        myBufferPos->type = 1;
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        myBufferPos->tCreate = milliseconds;
        semUp.sem_num = 3;
        semop(semId, &semUp, 1); //up first process
        semDown.sem_num = 3;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 14;
        semop(semId, &semDown, 1); //down assemble availability and then up, just a check to give paint priority
        semUp.sem_num = 14;
        semop(semId, &semUp, 1); //down assemble availability and then up, just a check to give paint priority
        semDown.sem_num = 6;
        semop(semId, &semDown, 1); //down second availability
        printf("Constructing component 1\n");
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 7; //up second process
        semop(semId, &semUp, 1);
        semUp.sem_num = 0;
        semop(semId, &semUp, 1); //up availability
    }
}

void constructSecond(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from construction component 2\n");
    struct sembuf semDown = {3, -1, 0};
    struct sembuf semUp = {3, 1, 0};
    int i=0;
    for (i=0;i<elements;i++) {
        int msPause = rand() % 50;
        struct timespec ts;
        ts.tv_sec = msPause / 1000;
        ts.tv_nsec = (msPause % 1000) * 1000000;
        nanosleep(&ts, NULL);
        semDown.sem_num = 1;
        semop(semId, &semDown, 1); //down first availability
        myBufferPos->id = rand() % 10000;
        myBufferPos->type = 2;
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        myBufferPos->tCreate = milliseconds;
        semUp.sem_num = 4;
        semop(semId, &semUp, 1); //up first process
        semDown.sem_num = 4;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 15;
        semop(semId, &semDown, 1); //down assemble availability and then up, just a check to give paint priority
        semUp.sem_num = 15;
        semop(semId, &semUp, 1); //down assemble availability and then up, just a check to give paint priority
        semDown.sem_num = 6;
        semop(semId, &semDown, 1); //down second availability
        printf("Constructing component 2\n");
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 7; //up second process
        semop(semId, &semUp, 1);
        semUp.sem_num = 1;
        semop(semId, &semUp, 1); //up availability
    }
}

void constructThird(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from construction component 3\n");
    struct sembuf semDown = {3, -1, 0};
    struct sembuf semUp = {3, 1, 0};
    int i=0;
    for (i=0;i<elements;i++) {
        int msPause = rand() % 50;
        struct timespec ts;
        ts.tv_sec = msPause / 1000;
        ts.tv_nsec = (msPause % 1000) * 1000000;
        nanosleep(&ts, NULL);
        semDown.sem_num = 2;
        semop(semId, &semDown, 1); //down first availability
        myBufferPos->id = rand() % 10000;
        myBufferPos->type = 3;
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        myBufferPos->tCreate = milliseconds;
        semUp.sem_num = 5;
        semop(semId, &semUp, 1); //up first process
        semDown.sem_num = 5;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 16;
        semop(semId, &semDown, 1); //down assemble availability and then up, just a check to give paint priority
        semUp.sem_num = 16;
        semop(semId, &semUp, 1); //down assemble availability and then up, just a check to give paint priority
        semDown.sem_num = 6;
        semop(semId, &semDown, 1); //down second availability
        printf("Constructing component 3\n");
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 7; //up second process
        semop(semId, &semUp, 1);
        semUp.sem_num = 2;
        semop(semId, &semUp, 1); //up availability
    }
}