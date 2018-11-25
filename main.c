#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <time.h>
#include "semconfig.h"
#include "construction.h"
#include "paint.h"
#include "check.h"
#include "assemble.h"

void initSems(int semId);

int main(int argc, char *argv[]) {
    int elements = 1000;
    if (argc == 2) {
        elements = atoi(argv[1]);
    }
    srand(time(NULL));
    //semaphore create
    int numOfSemaphores = 21;
    int semId = semget((key_t) 111, numOfSemaphores, IPC_CREAT | 0660);
    struct sembuf semDown = {0, -1, 0};
    struct sembuf semUp = {0, 1, 0};
    int i = 0;
    initSems(semId);

    //create shared memory segments
    // *2 + 1 because we keep type + id + buffer next free location at the end
    int constructionFirstBuffer = shmget((key_t) 101, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* cofp = shmat(constructionFirstBuffer, NULL, 0);
    int constructionSecondBuffer = shmget((key_t) 102, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* cosp = shmat(constructionSecondBuffer, NULL, 0);
    int constructionThirdBuffer = shmget((key_t) 103, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* cotp = shmat(constructionThirdBuffer, NULL, 0);
    int paintBuffer = shmget((key_t) 201, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* pp = shmat(paintBuffer, NULL, 0);
    int checkFirstBuffer = shmget((key_t) 301, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* chfp = shmat(checkFirstBuffer, NULL, 0);
    int checkSecondBuffer = shmget((key_t) 302, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* chsp = shmat(checkSecondBuffer, NULL, 0);
    int checkThirdBuffer = shmget((key_t) 303, sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* chtp = shmat(constructionThirdBuffer, NULL, 0);
    int assembleBuffer = shmget((key_t) 401, 3 * sizeof(struct cItem), IPC_CREAT | 0666);
    struct cItem* ap = shmat(assembleBuffer, NULL, 0);
    int paintBlockedTotalTime = shmget((key_t) 501, sizeof(double), IPC_CREAT | 0666);
    double* pbt = shmat(paintBlockedTotalTime, NULL, 0);
    int constructionTotalTime = shmget((key_t) 502, sizeof(double), IPC_CREAT | 0666);
    double* ct = shmat(constructionTotalTime, NULL, 0);
    *pbt = 0;
    *ct = 0;

    if (fork() == 0) { //first state child
        constructFirst(semId, cofp, pp, elements);
        return 0;
    }
    if (fork() == 0) { //first state child
        constructSecond(semId, cosp, pp, elements);
        return 0;
    }
    if (fork() == 0) { //first state child
        constructThird(semId, cotp, pp, elements);
        return 0;
    }
    if (fork() == 0) { //second state child
        paint(semId, pp, chfp, chsp, chtp, elements);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkFirst(semId, chfp, ap, elements);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkSecond(semId, chsp, ap, elements);
        return 0;
    }
    if (fork() == 0) { //third state child
        checkThird(semId, chtp, ap, elements);
        return 0;
    }
    if (fork() == 0) { //forth state child
        assemble(semId, ap, elements, ct, pbt);
        return 0;
    }

    int status;
    while (wait(&status) > 0); //wait for all children to finish
    wait(&status);

    printf("Average component blocked time before painting: %lf\n", (*pbt)/(elements*3));
    printf("Average component creation time: %lf\n", (*ct)/elements);

    for (i = 0; i < numOfSemaphores; i++) {
        semctl(semId, i, IPC_RMID, 0);
    }

    shmdt(cofp); shmdt(cosp); shmdt(cotp);
    shmdt(pp);
    shmdt(chfp); shmdt(chsp); shmdt(chtp);
    shmdt(ap);
    shmdt(pbt); shmdt(ct);

    shmctl(constructionFirstBuffer, 0, IPC_RMID);
    shmctl(constructionSecondBuffer, 0, IPC_RMID);
    shmctl(constructionThirdBuffer, 0, IPC_RMID);
    shmctl(paintBuffer, 0, IPC_RMID);
    shmctl(checkFirstBuffer, 0, IPC_RMID);
    shmctl(checkSecondBuffer, 0, IPC_RMID);
    shmctl(checkThirdBuffer, 0, IPC_RMID);
    shmctl(assembleBuffer, 0, IPC_RMID);
    shmctl(paintBlockedTotalTime, 0, IPC_RMID);
    shmctl(constructionTotalTime, 0, IPC_RMID);

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