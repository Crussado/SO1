#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include "filtro.h"

#define MOLS 10
#define VISITANTES 100000

/* Cantidad de visitantes en el Jardín */
long int visitantes;

filtro_t filtro;    

void *molinete(void *arg){

  long int id = (long int) arg;

  printf("Molinete %ld activo\n", id);

  for(int i = 0; i < VISITANTES; i++){
    filtro_lock(filtro, id);
	  //BEGIN: región crítica
    visitantes ++;
	  //END: región crítica
    filtro_unlock(filtro, id);
  }

  printf("Molinete %ld inactivo\n", id);
  pthread_exit(EXIT_SUCCESS);
}

int main(){
  pthread_t mols[MOLS];

  /* Abrimos el Jardín */
  visitantes = 0;

  /* Init */
  filtro = filtro_init(MOLS);

  /* Habilitamos los molinetes */
  for(int i = 0; i < MOLS; i++) {
      assert(!pthread_create( &mols[i]
                          , NULL
                          , molinete
                          , (void*) i));

  }

  /* Pasan personas */

  /* Se cierra el jardín */
  for(int i = 0; i < MOLS; i++)
    assert(! pthread_join(mols[i], NULL));

  filtro_destroy(filtro);

  printf("Visitantes que pasaron en el día %ld =?= %d\n", visitantes, MOLS * VISITANTES);

  return EXIT_SUCCESS;
}
