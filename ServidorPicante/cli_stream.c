#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>


#define MAXMSG 1024

int main(int argc, char* argv []) {
  if(argc != 2) {
    perror("Error en los datos iniciales");
    exit(EXIT_FAILURE);
  }

  int sock_cli;
  struct sockaddr_un srv_nombre;
  socklen_t size;
  ssize_t nbytes;
  char buff[MAXMSG+1], buff_Rta[MAXMSG+1];

  sock_cli = socket(AF_UNIX, SOCK_STREAM, 0);
  if(sock_cli < 0){
    perror("Falló la creación del socket");
    exit(EXIT_FAILURE);
  }

  /* Asignamos la dirección del servidor */
  srv_nombre.sun_family = AF_UNIX;
  strncpy(srv_nombre.sun_path, argv[1], sizeof(srv_nombre.sun_path));
  size = sizeof(struct sockaddr_un);
  /******************************************/

  if((connect(sock_cli, (struct sockaddr *) &srv_nombre, size))< 0){
    perror("Falló el intento de conexión con el servidor");
    close(sock_cli);
    exit(EXIT_FAILURE);
  }

  printf("Se establecio la conexion con el servidor\n");

  do {
    fgets(buff, MAXMSG, stdin);
    buff[strlen(buff)] = 0;
    nbytes = send(sock_cli, buff, MAXMSG, 0);
    if(nbytes < 0 ) 
      perror("Falló el envío del mensaje");
    else {
      nbytes = recv(sock_cli, buff_Rta, MAXMSG, 0);
      if(nbytes < 0)
        perror("Falló la recepción de un mensaje");
      else 
        printf("%s\n", buff_Rta);
    }

  }
  while(strcmp("exit\n", buff) != 0);

  printf("Fin de la conexion\n");
  close(sock_cli);
  return(EXIT_SUCCESS);
}
