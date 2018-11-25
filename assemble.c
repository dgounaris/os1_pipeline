#include "assemble.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>
#include <sys/time.h>
#include "semconfig.h"
#include <stdlib.h>

void assemble(int semId, struct cItem *myBufferPos, int elements, double* ct, double* pbt) {
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
        id1 = myBufferPos[0].id; id2 = myBufferPos[1].id; id3 = myBufferPos[2].id;
        struct timeval te;
        gettimeofday(&te, NULL); // get current time
        long sec = te.tv_sec;
        long msec = te.tv_usec;
        //make millis only their difference
        double interval = (sec-myBufferPos[0].sec)+(msec-myBufferPos[0].msec)/1000000.0;
        if ((sec-myBufferPos[1].sec)+(msec-myBufferPos[1].msec)/1000000.0 > interval) {
            interval = (sec-myBufferPos[1].sec)+(msec-myBufferPos[1].msec)/1000000.0;
        }
        if ((sec-myBufferPos[2].sec)+(msec-myBufferPos[2].msec)/1000000.0 > interval) {
            interval = (sec-myBufferPos[2].sec)+(msec-myBufferPos[2].msec)/1000000.0;
        }
        printf("ct is %lf\n", *ct);
        *ct = *ct + interval;
        *pbt = *pbt + myBufferPos[0].paintBlockedTime + myBufferPos[1].paintBlockedTime + myBufferPos[2].paintBlockedTime;
        printf("ctafter is %lf\n", *ct);
        printf("ASSEMBLED! IDS: %d %d %d\n", id1, id2, id3);
        printf("Time taken: %lf sec\n", interval);
        semUp.sem_num = 14;
        semop(semId, &semUp, 1); //up availability
        semUp.sem_num = 15;
        semop(semId, &semUp, 1); //up availability
        semUp.sem_num = 16;
        semop(semId, &semUp, 1); //up availability
    }
}