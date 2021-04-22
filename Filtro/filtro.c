#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include "filtro.h"

filtro_t filtro_init(unsigned int n) {
    filtro_t filtro;
    filtro.nivel = malloc(sizeof(int) * n);
    for(int i = 0; i < n; i++) 
        filtro.nivel[i] = -1;
    filtro.victimas = malloc(sizeof(int) * (n - 1));
    for(int i = 0; i < n - 1; i++) 
        filtro.victimas[i] = -1;
    filtro.n = n;
    return filtro;
}

void filtro_destroy(filtro_t filtro) {
    free(filtro.nivel);
    free(filtro.victimas);
}

int conseguir_bandera(filtro_t filtro, int id, int sala) {
    int bandera = 0;
    int i = 0;
    while (!bandera && i < filtro.n) {
        bandera = i != id && filtro.nivel[i] >= sala;
        i++;
    }

    return bandera;
}

void filtro_lock(filtro_t filtro, int id) {
    int bandera = 0;
    for(int i = 0; i < filtro.n - 1; i++) {
        filtro.nivel[id] = i;
        filtro.victimas[i] = id;
        bandera = conseguir_bandera(filtro, id, i);
        while (bandera && filtro.victimas[i] == id)
            bandera = conseguir_bandera(filtro, id, i);
    }
}

void filtro_unlock(filtro_t filtro, int id) {
    filtro.nivel[id] = -1;
}
