#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include "Barreras.h"

int barrier_init(barrier_t *barr, unsigned int count) {
    barr->n = count;
    barr->waiting = count - 1;

    barr->variable = malloc(sizeof(pthread_cond_t));
    pthread_cond_init(barr->variable, NULL);

    barr->mutex = malloc(sizeof(pthread_mutex_t) * (count - 1));
    for(unsigned int i = 0; i < count - 1; i++)
        pthread_mutex_init(&barr->mutex[i], NULL);

    barr->recursoContador = malloc(sizeof(pthread_mutex_t));
    pthread_mutex_init(barr->recursoContador, NULL);

    return 0;
}

int barrier_wait(barrier_t *barr) {
    pthread_mutex_lock(barr->recursoContador);
    if(barr->waiting != 0) {
        barr->waiting -= 1;
        int id = barr->waiting;
        pthread_mutex_unlock(barr->recursoContador);

        pthread_mutex_lock(&barr->mutex[id]);
        pthread_cond_wait(barr->variable, &barr->mutex[id]);
        pthread_mutex_unlock(&barr->mutex[id]);
    }
    else {
        barr->waiting = barr->n - 1;
        pthread_mutex_unlock(barr->recursoContador);
        pthread_cond_broadcast(barr->variable);
        pthread_cond_destroy(barr->variable);
        pthread_cond_init(barr->variable, NULL);
    }
    return 0;
}

int barrier_destroy(barrier_t *barr) {
    for(int i = 0; i < barr->n - 1; i++)
        pthread_mutex_destroy(&barr->mutex[i]);
    free(barr->mutex);
    pthread_cond_destroy(barr->variable);
    pthread_mutex_destroy(barr->recursoContador);
    free(barr->variable);
    free(barr->recursoContador);

    return 0;
}
