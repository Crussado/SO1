#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include "mutex.h"

#define MOLS 2
#define VISITANTES 1000000

long int visitantes;

void *molinete(void *arg){
  mutex *recurso = (mutex *) arg;

  for(int i = 0; i < VISITANTES; i++){

    mutex_lock(recurso);
    visitantes ++;
    mutex_unlock(recurso);
  }

  pthread_exit(EXIT_SUCCESS);
}

int main(){
  pthread_t mols[2];
  mutex recurso;
  mutex_init(&recurso);
  visitantes = 0;

  /* Habilitamos los molinetes */
  assert(!pthread_create( &mols[0]
                          , NULL
                          , molinete
                          , (void*) &recurso));

  assert(!pthread_create( &mols[1]
                          , NULL
                          , molinete
                          , (void*) &recurso));

  /* Pasan personas */

  /* Se cierra el jardín */
  assert(! pthread_join(mols[0], NULL));
  assert(! pthread_join(mols[1], NULL));
  mutex_destroy(&recurso);

  printf("Visitantes que pasaron en el día %ld =?= %d\n", visitantes, MOLS * VISITANTES);

  return EXIT_SUCCESS;
}
