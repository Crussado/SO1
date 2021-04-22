#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include "Semaforos.h"

int sem_init(sem_t *sem, int init) {
    sem->contador = init;
    pthread_mutex_init(&sem->recursoSemaforo, NULL);
    pthread_mutex_init(&sem->recursoContador, NULL);
    return 0;
}

int sem_incr(sem_t *sem) {
    pthread_mutex_lock(&(sem->recursoContador));
    if(sem->contador <= 0)
        pthread_mutex_unlock(&(sem->recursoSemaforo));

    sem->contador += 1;
    pthread_mutex_unlock(&(sem->recursoContador));

    return sem->contador;
}

int sem_decr(sem_t *sem) {
    pthread_mutex_lock(&(sem->recursoContador));
    sem->contador -= 1;

    if(sem->contador <= 0)
        pthread_mutex_lock(&(sem->recursoSemaforo));

    pthread_mutex_unlock(&(sem->recursoContador));

    return sem->contador;
}

int sem_destroy(sem_t *sem) {
    pthread_mutex_destroy(&sem->recursoSemaforo);
    pthread_mutex_destroy(&sem->recursoContador);
    return 0;
}