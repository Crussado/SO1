#include <stdlib.h>
#include <stdio.h>
#include "Slist.h"

SList slist_crear() {
  return NULL;
}

void eliminar_nodo(SNodo * nodo) {
  free(nodo);
}

void slist_destruir(SList lista) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    eliminar_nodo(nodoAEliminar);
  }
}

int slist_longitud(SList lista){
  int cont=0;
  for(; lista!=NULL; lista=lista->sig, cont++);
  return cont;
}

int slist_vacia(SList lista) {
  return lista == NULL;
}

SList slist_agregar_final(SList lista, int x, int y, char state) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->x = x;
  nuevoNodo->y = y;
  nuevoNodo->state = state;
  nuevoNodo->sig = NULL;

  if (slist_vacia(lista))
    return nuevoNodo;

  SList nodo = lista;
  int esta = 0;
  for (; nodo->sig != NULL && !esta; nodo = nodo->sig)
    if(nodo->x == x && nodo->y == y)
      esta = 1;

  if(esta || (nodo->x == x && nodo->y == y)) {
    eliminar_nodo(nuevoNodo);
    return lista;
  }

  nodo->sig = nuevoNodo;

  return lista;
}

SList pop(SList lista, int *x, int *y, char *state) {
  if(!slist_vacia(lista)){
    *x = lista->x;
    *y = lista->y;
    *state = lista->state;
    SList nodoAEliminar = lista;
    lista = lista->sig;
    eliminar_nodo(nodoAEliminar);
  }

  return lista;
}

SList push(SList lista, int x, int y, char state) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->x = x;
  nuevoNodo->y = y;
  nuevoNodo->state = state;
  nuevoNodo->sig = lista;
  return nuevoNodo;
}

void slist_print(SList lista) {
  if(!slist_vacia(lista)) {
    SList nodo = lista;
    for(; nodo != NULL; nodo = nodo->sig)
      printf("x: %d y: %d - state: %c\n", nodo->x, nodo->y, nodo->state);
  }
}