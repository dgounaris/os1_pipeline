#include "check.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include "semconfig.h"
#include <sys/sem.h>
#include <time.h>

void checkFirst(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from checking component 1\n");
    struct sembuf semDown = {11, -1, 0};
    struct sembuf semUp = {11, 1, 0};
    int pickPos = 0;
    int putPos = 0;
    int id, type;
    int i=0;
    for (i=0;i<elements;i++) {
        semDown.sem_num = 11;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 14;
        semop(semId, &semDown, 1); //down fourth availability
        printf("Checking component 1\n");
        struct timespec ts;
        ts.tv_sec = 60 / 1000;
        ts.tv_nsec = (60 % 1000) * 1000000;
        nanosleep(&ts, NULL);
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 17; //up fourth process
        semop(semId, &semUp, 1);
        semUp.sem_num = 8;
        semop(semId, &semUp, 1); //up availability
    }
}

void checkSecond(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from checking component 2\n");
    struct sembuf semDown = {11, -1, 0};
    struct sembuf semUp = {11, 1, 0};
    int pickPos = 0;
    int putPos = 0;
    int id, type;
    int i=0;
    for (i=0;i<elements;i++) {
        semDown.sem_num = 12;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 15;
        semop(semId, &semDown, 1); //down fourth availability
        printf("Checking component 2\n");
        struct timespec ts;
        ts.tv_sec = 60 / 1000;
        ts.tv_nsec = (60 % 1000) * 1000000;
        nanosleep(&ts, NULL);
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 18; //up fourth process
        semop(semId, &semUp, 1);
        semUp.sem_num = 9;
        semop(semId, &semUp, 1); //up availability
    }
}

void checkThird(int semId, struct cItem *myBufferPos, struct cItem *nextBufferPos, int elements) {
    printf("Hello from checking component 3\n");
    struct sembuf semDown = {11, -1, 0};
    struct sembuf semUp = {11, 1, 0};
    int pickPos = 0;
    int putPos = 0;
    int id, type;
    int i=0;
    for (i=0;i<elements;i++) {
        semDown.sem_num = 13;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        semDown.sem_num = 16;
        semop(semId, &semDown, 1); //down fourth availability
        printf("Checking component 3\n");
        struct timespec ts;
        ts.tv_sec = 60 / 1000;
        ts.tv_nsec = (60 % 1000) * 1000000;
        nanosleep(&ts, NULL);
        nextBufferPos->id = myBufferPos->id;
        nextBufferPos->type = myBufferPos->type;
        nextBufferPos->tCreate = myBufferPos->tCreate;
        semUp.sem_num = 19; //up fourth process
        semop(semId, &semUp, 1);
        semUp.sem_num = 10;
        semop(semId, &semUp, 1); //up availability
    }
}