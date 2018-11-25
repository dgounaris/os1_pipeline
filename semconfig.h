#ifndef ASKISI1_HATZ_CONFIG_H
#define ASKISI1_HATZ_CONFIG_H

#include "semconfig.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/wait.h>

union semun {
    int val;
    struct semid_ds *bif;
    unsigned short *array;
};

struct cItem {
    int id;
    int type;
    long sec;
    long msec;
    double paintBlockedTime;
};

#endif //ASKISI1_HATZ_CONFIG_H
