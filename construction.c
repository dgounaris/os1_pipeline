#include "construction.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void constructFirst(int semId) {
    printf("Hello from construction component 1\n");
    struct sembuf semDown = {3, -1, 0};
    struct sembuf semUp = {3, 1, 0};
    //todo loop this
    int i=0;
    for (i=0;i<2;i++) {
        semDown.sem_num = 3;
        semop(semId, &semDown, 1); //wait for process signal and start processing
        printf("Constructing component 1\n");
        //todo propagate here
        semDown.sem_num = 6;
        semop(semId, &semDown, 1); //down second availability
        semUp.sem_num = 7; //up second process
        semop(semId, &semUp, 1);
        semUp.sem_num = 0;
        semop(semId, &semUp, 1); //up availability
    }
}

void constructSecond(int semId) {
    printf("Hello from construction component 2\n");

}

void constructThird(int semId) {
    printf("Hello from construction component 3\n");

}