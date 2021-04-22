#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>>

#define HILOS 2
pthread_mutex_t mutex[2];
pthread_cond_t cond;

void* esperar(void* arg) {
   int i = (int) arg;
   printf("Hilo %d inicia espera\n", i);
   pthread_cond_wait(&cond, &mutex[i]);
   printf("Hilo %d termina\n", i);
}

int main() {
    int i;
    pthread_t hilos[HILOS];
    pthread_cond_init(&cond, NULL);
    pthread_mutex_init(&mutex[0], NULL);
    pthread_mutex_init(&mutex[1], NULL);
    for (i=0;i<HILOS;i++)
        pthread_create(&hilos[i], NULL, esperar, (void *)i);

    printf("ingrese algo:\n");
    sleep(3);
    pthread_cond_broadcast(&cond);
    printf("aaaaaaaaa\n");

    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    return 0;
}