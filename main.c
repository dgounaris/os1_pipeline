#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include "semconfig.h"
#include "construction.h"
#include "paint.h"
#include "check.h"
#include "assemble.h"

void initSems(int semId);

int main() {
    //semaphore create
    int numOfSemaphores = 21;
    int semId = semget((key_t) 111, numOfSemaphores, IPC_CREAT | 0660);
    struct sembuf semDown = {0, -1, 0};
    struct sembuf semUp = {0, 1, 0};
    int i = 0;
    initSems(semId);

    if (fork() == 0) { //first state child
        constructFirst(semId);
        return 0;
    }
    if (fork() == 0) { //first state child
        constructSecond(semId);
        return 0;
    }
    if (fork() == 0) { //first state child
        constructThird(semId);
        return 0;
    }
    if (fork() == 0) { //second state child
        paint(semId);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkFirst(semId);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkSecond(semId);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkThird(semId);
        return 0;
    }
    if (fork() == 0) { //forth state child
        assemble(semId);
        return 0;
    }

    semop(semId, &semDown, 1); //down first availability
    semUp.sem_num = 3;
    semop(semId, &semUp, 1); //up first process
    semop(semId, &semDown, 1); //down first availability
    semUp.sem_num = 3;
    semop(semId, &semUp, 1); //up first process

    int status;
    while (wait(&status) > 0); //wait for all children to finish
    wait(&status);
    for (i = 0; i < numOfSemaphores; i++) {
        semctl(semId, i, IPC_RMID, 0);
    }
    return 0;
}

void initSems(int semId) {
    //todo here is the init, not in generatesemops
    union semun arg;
    int i = 0;
    //first state availability
    arg.val = 1;
    for (i = 0; i < 3; i++) {
        semctl(semId, i, SETVAL, arg);
    }
    //first state process status
    arg.val = 0;
    for (i = 3; i < 6; i++) {
        semctl(semId, i, SETVAL, arg);
    }
    //second state availability
    arg.val = 1;
    semctl(semId, 6, SETVAL, arg);
    //second state process status
    arg.val = 0;
    semctl(semId, 7, SETVAL, arg);
    //third state availability
    arg.val = 1;
    for (i = 8; i < 11; i++) {
        semctl(semId, i, SETVAL, arg);
    }
    //third state process status
    arg.val = 0;
    for (i = 11; i < 14; i++) {
        semctl(semId, i, SETVAL, arg);
    }
    //fourth state availability
    arg.val = 1; //item 1
    semctl(semId, 14, SETVAL, arg);
    arg.val = 1; //item 2
    semctl(semId, 15, SETVAL, arg);
    arg.val = 1; //item 3
    semctl(semId, 16, SETVAL, arg);
    //fourth state process status
    arg.val = 0;
    semctl(semId, 17, SETVAL, arg);
    semctl(semId, 18, SETVAL, arg);
    semctl(semId, 19, SETVAL, arg);
    //still pending items semaphore
    arg.val = 1;
    semctl(semId, 20, SETVAL, arg);
}