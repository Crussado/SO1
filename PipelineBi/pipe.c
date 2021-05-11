#include <stdio.h> /* Print */
#include <unistd.h> /* Read/Write + Fork */
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "pipeBi.h"

int main(){
  pipeBi* tuberia = crear_PipeBi();
  char buffP[BUFFER];

  /* Fork */
  pid_t p = fork();

  if (p == 0){ /* Child */
    setear(tuberia, p);
    strcpy(buffP, "culo");
    enviar (tuberia, "aaaaaaaaaaaaaaa", p);
    enviar (tuberia, "buffP", p);
    enviar (tuberia, "XD", p);
    recibir(tuberia, buffP, p);
    printf("padre:%s\n", buffP);
    destruir(tuberia,p);
    exit(EXIT_SUCCESS);

  } else { /* Parent */
    setear(tuberia, p);
    strcpy(buffP, "holis");
    enviar (tuberia, buffP, p);
    sleep(1);
    recibir(tuberia, buffP, p);
    printf("hijo:%s\n", buffP);
    destruir(tuberia,p);
    exit(EXIT_SUCCESS);
    
  }
  /* Código muerto */
  exit(EXIT_SUCCESS);
}
