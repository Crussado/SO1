#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#define HILOS 3
pthread_barrier_t barrera;

void* esperar(void* arg) {
   int i = (int) arg;
   printf("Hilo %d inicia espera\n", i);
   pthread_barrier_wait(&barrera);
   printf("Hilo %d termina\n", i);
}

int main(){
    int i;
    pthread_t hilos[HILOS];
    pthread_barrier_init(&barrera, NULL, HILOS);
    for (i=0;i<HILOS;i++)
        pthread_create(&hilos[i], NULL, esperar, (void *)i);
    
    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    pthread_join(hilos[2], NULL);
    pthread_barrier_destroy(&barrera);
    return 0;
}