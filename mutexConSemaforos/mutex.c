#include <semaphore.h>
#include "mutex.h"
#include <stdlib.h>

int mutex_init(mutex *m) {
    sem_init(&m->sem, 0, 2);
    return 0;
}

int mutex_lock(mutex *m) {
    sem_wait(&m->sem);
    return 0;
}

int mutex_unlock(mutex *m) {
    int i;
    sem_getvalue(&m->sem, &i);
    if (i != 0)
        sem_post(&m->sem);
    return 0;
}

int mutex_destroy(mutex *m) {
    sem_destroy(&m->sem);
    return 0;
}