#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include "Semaforos.h"

#define MOLS 2
#define VISITANTES 1000000

long int visitantes;

void *molinete(void *arg){
  sem_d *semaforo = (sem_d *) arg;

  for(int i = 0; i < VISITANTES; i++){

    semd_decr(semaforo);
    visitantes ++;
    semd_incr(semaforo);
  }

  pthread_exit(EXIT_SUCCESS);
}

int main(){
  pthread_t mols[2];
  sem_d semaforo;
  semd_init(&semaforo, 1);
  visitantes = 0;

  /* Habilitamos los molinetes */
  assert(!pthread_create( &mols[0]
                          , NULL
                          , molinete
                          , (void*) &semaforo));

  assert(!pthread_create( &mols[1]
                          , NULL
                          , molinete
                          , (void*) &semaforo));

  /* Pasan personas */

  /* Se cierra el jardín */
  assert(! pthread_join(mols[0], NULL));
  assert(! pthread_join(mols[1], NULL));
  semd_destroy(&semaforo);

  printf("Visitantes que pasaron en el día %ld =?= %d\n", visitantes, MOLS * VISITANTES);

  return EXIT_SUCCESS;
}
