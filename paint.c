#include "paint.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semconfig.h"
#include <time.h>

void paint(int semId, struct cItem *myBufferPos, struct cItem *nextFirstBufferPos, struct cItem *nextSecondBufferPos,
           struct cItem *nextThirdBufferPos, int elements) {
    printf("Hello from painting component\n");
    struct sembuf semDown = {7, -1, 0};
    struct sembuf semUp = {7, 1, 0};
    int id, type;
    printf("Paint pos: %p Next: %p\n", myBufferPos, nextFirstBufferPos);
    int i=0;
    for (i=0;i<elements*3;i++) {
        semDown.sem_num = 7;
        semop(semId, &semDown, 1); //down second process
        type = myBufferPos->type;
        printf("Painting component\n");
        if (type == 1) {
            struct timespec ts;
            ts.tv_sec = 15 / 1000;
            ts.tv_nsec = (15 % 1000) * 1000000;
            nanosleep(&ts, NULL);
        } else if (type == 2) {
            struct timespec ts;
            ts.tv_sec = 20 / 1000;
            ts.tv_nsec = (20 % 1000) * 1000000;
            nanosleep(&ts, NULL);
        } else if (type == 3) {
            struct timespec ts;
            ts.tv_sec = 30 / 1000;
            ts.tv_nsec = (30 % 1000) * 1000000;
            nanosleep(&ts, NULL);
        }
        printf("ptype %d\n", type);
        if (type == 1) {
            semDown.sem_num = 8;
            semop(semId, &semDown, 1); //down third availability
            nextFirstBufferPos->id = myBufferPos->id;
            nextFirstBufferPos->type = myBufferPos->type;
            nextFirstBufferPos->sec = myBufferPos->sec;
            nextFirstBufferPos->msec = myBufferPos->msec;
            nextFirstBufferPos->paintBlockedTime = myBufferPos->paintBlockedTime;
            semUp.sem_num = 11;
            semop(semId, &semUp, 1); //up third process
        } else if (type == 2) {
            semDown.sem_num = 9;
            semop(semId, &semDown, 1); //down third availability
            nextSecondBufferPos->id = myBufferPos->id;
            nextSecondBufferPos->type = myBufferPos->type;
            nextSecondBufferPos->sec = myBufferPos->sec;
            nextSecondBufferPos->msec = myBufferPos->msec;
            nextSecondBufferPos->paintBlockedTime = myBufferPos->paintBlockedTime;
            semUp.sem_num = 12;
            semop(semId, &semUp, 1); //up third process
        } else if (type == 3) {
            semDown.sem_num = 10;
            semop(semId, &semDown, 1); //down third availability
            nextThirdBufferPos->id = myBufferPos->id;
            nextThirdBufferPos->type = myBufferPos->type;
            nextThirdBufferPos->sec = myBufferPos->sec;
            nextThirdBufferPos->msec = myBufferPos->msec;
            nextThirdBufferPos->paintBlockedTime = myBufferPos->paintBlockedTime;
            semUp.sem_num = 13;
            semop(semId, &semUp, 1); //up third process
        }
        semUp.sem_num = 6;
        semop(semId, &semUp, 1); //up availability
    }
}