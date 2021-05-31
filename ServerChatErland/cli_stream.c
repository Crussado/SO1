/* Cabeceras de Sockets */
#include <sys/socket.h>
#include <sys/un.h>
/**********/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
/**********/
/* Cabecera de direcciones por red */
#include <netdb.h>
/* PThread */
#include <pthread.h> 
/**********/
#include "cli_stream.h"

#define COMANDO_MEN "/msg"
#define COMANDO_SALIDA "/exit"
#define COMANDO_NICK "/nickname"
#define MAXMSG 1024

void error(int *socket, struct addrinfo *resultado, char *printError ) {
  if(socket != NULL)
    close(*socket);
  if(resultado != NULL)
    freeaddrinfo(resultado);
  perror(printError);
  exit(EXIT_FAILURE);
}

void* enviar(void* arg) {
  char buff[MAXMSG];
  ssize_t nbytes;
  int sock_cli = (int) arg;
  do {
    fgets(buff, MAXMSG, stdin); 
    buff[strlen(buff) - 1] = 0;

    nbytes = send(sock_cli, buff, MAXMSG, 0);
    if(nbytes < 0 ) 
      perror("Falló el envío del mensaje.");
  }
  while(strcmp(COMANDO_SALIDA, buff) != 0);
}

void* recibir(void* arg) {
  char buff[MAXMSG];
  ssize_t nbytes;
  int sock_cli = (int) arg;
  do {
    nbytes = recv(sock_cli, buff, MAXMSG, 0);
    if(nbytes < 0)
      perror("Falló la recepción de un mensaje");
    else 
      printf("%s\n", buff);
  }
  while(strcmp(COMANDO_SALIDA, buff) != 0);
}

int main(int argc, char* argv []) {
  if(argc != 3)
    error(NULL, NULL, "Error en los datos iniciales");

  int sock_cli;
  struct addrinfo *resultado;
  pthread_t envio, recepcion;

  /* Buscamos la dirección del hostname:port */
  if (getaddrinfo(argv[1], argv[2], NULL, &resultado)){
    fprintf(stderr,"No se encontro el host: %s \n",argv[1]);
    exit(EXIT_FAILURE);
  }

  sock_cli = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_cli < 0)
    error(NULL, resultado, "Error en los datos iniciales");

  if(connect(sock_cli, (struct sockaddr *) resultado->ai_addr, resultado->ai_addrlen) != 0)
      error(&sock_cli, resultado, "No se pudo conectar");

  printf("Se establecio la conexion con el servidor\n");
  
  pthread_create(&envio, NULL, enviar, (void *) sock_cli);
  pthread_create(&recepcion, NULL, recibir, (void *) sock_cli);

  pthread_join(recepcion, NULL);
  pthread_join(envio, NULL);

  printf("Fin de la conexion\n");
  freeaddrinfo(resultado);
  close(sock_cli);
  return(EXIT_SUCCESS);
}
