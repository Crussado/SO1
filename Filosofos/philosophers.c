#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define N_FILOSOFOS 5
#define ESPERA 5000000

pthread_mutex_t tenedor[N_FILOSOFOS];

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
  if(i == 0) {
    pthread_mutex_lock(&tenedor[(i+1)%N_FILOSOFOS]); /* Toma el tenedor a su izquierda */  
    pthread_mutex_lock(&tenedor[i]); /* Toma el tenedor a su derecha */
  }
  else {
    pthread_mutex_lock(&tenedor[i]); /* Toma el tenedor a su derecha */
    pthread_mutex_lock(&tenedor[(i+1)%N_FILOSOFOS]); /* Toma el tenedor a su izquierda */
  }
}

void dejar_tenedores(int i)
{
  if(i == 0) {
    pthread_mutex_unlock(&tenedor[(i+1)%N_FILOSOFOS]); /* Deja el tenedor de su izquierda */
    pthread_mutex_unlock(&tenedor[i]); /* Deja el tenedor de su derecha */
  }
  else {
    pthread_mutex_unlock(&tenedor[i]); /* Deja el tenedor de su derecha */
    pthread_mutex_unlock(&tenedor[(i+1)%N_FILOSOFOS]); /* Deja el tenedor de su izquierda */
  }
}

void *filosofo(void *arg)
{
  int i = (int)arg;
  for (;;)
  {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);
  }
}

int main()
{
  int i;
  pthread_t filo[N_FILOSOFOS];
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_mutex_init(&tenedor[i], NULL);
  for (i=0;i<N_FILOSOFOS;i++)
    pthread_create(&filo[i], NULL, filosofo, (void *)i);
  pthread_join(filo[0], NULL);
  return 0;
}

/* Este programa puede terminar en *deadlock*. ¿En qué situación se
puede dar?*/

/*Puede terminar en deadlock cuando, por ejemplo, los 5 filosofos quieren comer al
mismo tiempo ya que todos agarrarian su respectivo tenedor pero no podrian tomar el tenedor faltante
puesto que lo estaria usando otro filosofo.*/

/*Cansados de no comer los filósofos deciden pensar una solución a su problema.
Uno razona que esto no sucederá si alguno de ellos fuese zurdo y tome primero el
tenedor de su izquierda. Implemente esta solución y explique por qué funciona.*/

/*Dicha solucion si funcionaria ya que lo que haria seria que el filosofo zurdo coma y al dejar los
tenedores este libere el derecho haciendo que el filosofo siguiente que es derecho tome el tenedor
faltante y pueda comer. Y asi sucesivamente con los demas filosofos. Esta idea hara que un filosofo coma
y los demas lo esperen cosa de que al finalizar de comer se pasen el tenedor faltante.*/