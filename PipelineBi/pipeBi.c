#include <stdio.h> /* Print */
#include <unistd.h> /* Read/Write + Fork */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "pipeBi.h"

pipeBi* crear_PipeBi() {
    pipeBi *estructura = malloc(sizeof(pipeBi));
    pipe(estructura->fd1);
    pipe(estructura->fd2);
    return estructura;
}

void setear(pipeBi* tuberia, pid_t p) {
    if(p > 0) {
        close(tuberia->fd1[0]);
        close(tuberia->fd2[1]);
    }
    else {
        close(tuberia->fd1[1]);
        close(tuberia->fd2[0]);
    }
}

void enviar (pipeBi* tuberia, char* mensaje, pid_t p){
    if (p > 0) 
        /* Escribimos un mensaje al  */
        write(tuberia->fd1[1], mensaje, strlen(mensaje));
    else
        /* Escribimos un mensaje al  */
        write(tuberia->fd2[1], mensaje, strlen(mensaje));

}

void recibir (pipeBi* tuberia, char* buffer, pid_t p) {
    int rd;
    if(p > 0) {
        /* Leemos el mensaje, bloqueante */
        rd = read(tuberia->fd2[0], buffer, BUFFER);
        buffer[rd]= 0;
    }
    else {
        /* Leemos el mensaje, bloqueante */
        rd = read(tuberia->fd1[0], buffer, BUFFER);
        buffer[rd]= 0;
    }
}

void destruir(pipeBi* tuberia, pid_t p) {
    if(p > 0) {
        close(tuberia->fd1[1]);
        close(tuberia->fd2[0]);
    }
    else {
        close(tuberia->fd1[0]);
        close(tuberia->fd2[1]);
    }
    free(tuberia);
}
