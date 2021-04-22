#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <semaphore.h>
#include "Semaforos.h"

int semd_init(sem_d *sem, int init) {
    sem->contador = init;
    sem_init(&sem->sem, 0, init);
    pthread_mutex_init(&sem->recursoContador, NULL);

    return 0;
}

int semd_incr(sem_d *sem) {
    pthread_mutex_lock(&(sem->recursoContador));
    sem->contador += 1;
    pthread_mutex_unlock(&(sem->recursoContador));

    sem_post(&sem->sem);
    return 0;
}

int semd_decr(sem_d *sem) {
    pthread_mutex_lock(&(sem->recursoContador));
    sem->contador -= 1;
    pthread_mutex_unlock(&(sem->recursoContador));

    sem_wait(&sem->sem);
    return 0;
}


int semd_destroy(sem_d *sem) {
    sem_destroy(&sem->sem);
    return 0;
}