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
        semDown.sem_num = 17;
        semop(semId, &semDown, 1); //wait for process signal
        //todo uncomment when all 3 implemented
        //semDown.sem_num = 18;
        //semop(semId, &semDown, 1); //wait for process signal
        //semDown.sem_num = 19;
        //semop(semId, &semDown, 1); //wait for process signal
        printf("Assembling!\n");
        //todo process the assembly of the parts
        //todo this semUp for all availabilities (14 to 16)
        semUp.sem_num = 14;
        semop(semId, &semUp, 1); //up availability
    }
}