#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>


#define N_FILOSOFOS 5
#define ESPERA 5000000

pthread_mutex_t tenedor[N_FILOSOFOS];
sem_t comedores;

void pensar(int i)
{
  printf("Filosofo %d pensando...\n",i);
  usleep(random() % ESPERA);
}

void comer(int i)
{
  printf("Filosofo %d comiendo...\n",i);
  usleep(random() % ESPERA);
}

void tomar_tenedores(int i)
{
    pthread_mutex_lock(&tenedor[i]); /* Toma el tenedor a su derecha */
    pthread_mutex_lock(&tenedor[(i+1)%N_FILOSOFOS]); /* Toma el tenedor a su izquierda */
}

void dejar_tenedores(int i)
{
    pthread_mutex_unlock(&tenedor[i]); /* Deja el tenedor de su derecha */
    pthread_mutex_unlock(&tenedor[(i+1)%N_FILOSOFOS]); /* Deja el tenedor de su izquierda */
}

void *filosofo(void *arg)
{
  int i = (int)arg;
  for (;;)
  {
    sem_wait(&comedores);
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    sem_post(&comedores);
    pensar(i);
  }
}

int main()
{
  int i;
  pthread_t filo[N_FILOSOFOS];
  sem_init(&comedores, 0, N_FILOSOFOS - 1);
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_create(&filo[i], NULL, filosofo, (void *)i);
  pthread_join(filo[0], NULL);
  return 0;
}
