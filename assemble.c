#include "assemble.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/time.h>
#include "semconfig.h"
#include <stdlib.h>

void assemble(int semId, struct cItem *myBufferPos, int elements) {
    printf("Hello from assembling component\n");
    struct sembuf semDown = {0, -1, 0};
    struct sembuf semUp = {0, 1, 0};
    int id1, id2, id3;
    int i=0;
    for (i=0;i<elements;i++) {
        semDown.sem_num = 17;
        semop(semId, &semDown, 1); //wait for process signal
        semDown.sem_num = 18;
        semop(semId, &semDown, 1); //wait for process signal
        semDown.sem_num = 19;
        semop(semId, &semDown, 1); //wait for process signal
        struct timespec ts;
        ts.tv_sec = 100 / 1000;
        ts.tv_nsec = (100 % 1000) * 1000000;
        nanosleep(&ts, NULL);
        printf("Assembling!\n");
        id1 = myBufferPos->id; id2 = myBufferPos->id; id3 = myBufferPos->id;
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long long milliseconds = te.tv_sec*1000LL + te.tv_usec/1000; // calculate milliseconds
        long long interval = milliseconds - myBufferPos[0].tCreate;
        if (milliseconds - myBufferPos[1].tCreate > interval) {
            interval = milliseconds - myBufferPos[1].tCreate;
        }
        if (milliseconds - myBufferPos[2].tCreate > interval) {
            interval = milliseconds - myBufferPos[2].tCreate;
        }
        printf("ASSEMBLED! IDS: %d %d %d\n", id1, id2, id3);
        printf("Time taken: %lld ms\n", interval);
        semUp.sem_num = 14;
        semop(semId, &semUp, 1); //up availability
        semUp.sem_num = 15;
        semop(semId, &semUp, 1); //up availability
        semUp.sem_num = 16;
        semop(semId, &semUp, 1); //up availability
    }
}