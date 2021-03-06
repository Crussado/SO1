#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h> /* strncpy */

int crear_socket_nombrado(const char * archivo){
  int sock;
  struct sockaddr_un nombre;
  socklen_t size;

  /* Creación del socket */
  sock = socket(AF_UNIX, SOCK_DGRAM, 0);
  if(sock < 0){
    perror("Falló la creación del socket ");
    exit(EXIT_FAILURE);
  }

  /* Creamos la estructura de la dirección del socket */
  nombre.sun_family = AF_UNIX;
  strncpy(nombre.sun_path, archivo, sizeof(nombre.sun_path));
  size = sizeof(nombre);

  /* Asignación del nombre */
  if( (bind(sock, (struct sockaddr*) & nombre , size)) < 0 ){
    perror("Falló la asignación de nombre");
    exit(EXIT_FAILURE);
  }

  return sock;
}

int rm_socket_nombrado(const char *archivo, int socket){
  close(socket);
  return remove(archivo);
}
