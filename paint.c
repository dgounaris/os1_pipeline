#include "paint.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void paint(int semId) {
    printf("Hello from painting component\n");
    struct sembuf semDown = {7, -1, 0};
    struct sembuf semUp = {7, 1, 0};
    //todo loop this
    int i=0;
    for (i=0;i<2;i++) {
        semDown.sem_num = 7;
        semop(semId, &semDown, 1); //down second process
        printf("Painting component\n");
        semDown.sem_num = 8;
        semop(semId, &semDown, 1); //down third availability
        semUp.sem_num = 11;
        semop(semId, &semUp, 1); //up third process
        semUp.sem_num = 6;
        semop(semId, &semUp, 1); //up availability
    }
}