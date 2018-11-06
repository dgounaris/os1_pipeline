#include "check.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void checkFirst(int semId) {
    printf("Hello from checking component 1\n");
    struct sembuf semDown = {11, -1, 0};
    struct sembuf semUp = {11, 1, 0};
    //todo loop this
    int i=0;
    for (i=0;i<2;i++) {
        semDown.sem_num = 11;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        printf("Checking component 1\n");
        //todo propagate here
        semDown.sem_num = 14;
        semop(semId, &semDown, 1); //down fourth availability
        //todo if all 3, up fourth process
        semUp.sem_num = 17; //up fourth process
        semop(semId, &semUp, 1);
        semUp.sem_num = 8;
        semop(semId, &semUp, 1); //up availability
    }
}

void checkSecond(int semId) {
    printf("Hello from checking component 2\n");
}

void checkThird(int semId) {
    printf("Hello from checking component 3\n");
}