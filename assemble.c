#include "assemble.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

void assemble(int semId) {
    printf("Hello from assembling component\n");
    struct sembuf semDown = {0, -1, 0};
    struct sembuf semUp = {0, 1, 0};
    int i=0;
    for (i=0;i<2;i++) {
        //printf("-------- %d\n",semctl(semId, 17, GETVAL, NULL));
        semDown.sem_num = 17;
        semop(semId, &semDown, 1); //wait for process signal
        printf("Assembling!\n");
        //todo possibly create process signals for all 3 components
        //todo process the assembly of the parts
        //todo this semUp for all availabilities (14 to 16)
        semUp.sem_num = 14;
        semop(semId, &semUp, 1); //up availability
    }
}