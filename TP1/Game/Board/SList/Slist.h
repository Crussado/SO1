#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (int dato);

typedef struct _SNodo {
  int x;
  int y;
  char state;
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

/**
 * Devuelve una lista vacía.
 */
SList slist_crear();

/**
 * Destruccion de la lista.
 */
void slist_destruir(SList lista);

/**
 * Determina si la lista es vacía.
 */
int slist_vacia(SList lista);

/**
 * Agrega un elemento al final de la lista.
 */
SList slist_agregar_final(SList lista, int x, int y, char state);

/**
 * Agrega un elemento al inicio de la lista.
 */
SList push(SList lista, int x, int y, char state);

SList pop(SList lista, int *x, int *y, char *state);

int slist_longitud(SList lista);

void slist_print(SList lista);

#endif /* __SLIST_H__ */
