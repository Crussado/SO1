#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h> /* PThread */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h> /* Signals */

#define BLOG 1
#define MAXMSG 1024
sem_t cantAtendidos;
int sock_srv;
char nombreSocket[255];

void handler(int arg){
  close(sock_srv);
  sem_destroy(&cantAtendidos);
  remove(nombreSocket);

  printf("\nServer off\n");
  exit(EXIT_SUCCESS);
}

void* atender(void* arg) {
  ssize_t nbytes;
  char buff[MAXMSG];
  int sock_cli = (int) arg;

  printf("Conexion establecida\n");

  do {
    nbytes = recv(sock_cli, buff, MAXMSG, 0);
    if(nbytes < 0 )
      perror("Falló recepcion del mensaje");
    else {
      printf("Recibido\n");
      nbytes = send(sock_cli, "ACK", 3, 0);
      if(nbytes < 0 )
        perror("Falló el envío del ACK");
    }
  }
  while(strcmp("exit\n", buff) != 0);

  printf("Conexion terminada\n");
  sem_post(&cantAtendidos);
  close(sock_cli);
}

int main(int argc, char* argv[]){
  if(argc != 2) {
    perror("Error en los datos iniciales");
    exit(EXIT_FAILURE);
  }

  sem_init(&cantAtendidos, 0, 1);
  int sock_cli;
  struct sockaddr_un srv_nombre, cli_nombre;
  socklen_t size;
  char buff[MAXMSG+1];
  pthread_t clientes[3];
  int contador = 0;
  strcpy(nombreSocket, argv[1]);
  /* Creación de Socket Servidor */
  sock_srv = socket(AF_UNIX, SOCK_STREAM, 0);
  if(sock_srv < 0){
    perror("Falló la creación del socket");
    exit(EXIT_FAILURE);
  }

  /* Asignamos la dirección del servidor */
  srv_nombre.sun_family = AF_UNIX;
  strncpy(srv_nombre.sun_path, argv[1], sizeof(srv_nombre.sun_path));
  size = sizeof(struct sockaddr_un);

  if(bind(sock_srv, (struct sockaddr *) &srv_nombre, size)){
    perror("Falló la asignación del nombre del servidor");
    exit(EXIT_FAILURE);
  }

  printf("Server on\n");

  /* El servidor se pone a la espera de conexiones */
  if(listen(sock_srv, BLOG) < 0){
    perror("Falló el listen");
    exit(EXIT_FAILURE);
  }

  printf("Listen\n");
  signal(SIGINT, handler);

  while(1) {
    /* Apareció una conexión :D! */
    sem_wait(&cantAtendidos);
    sock_cli = accept(sock_srv, (struct sockaddr *) &cli_nombre, & size);
    if(sock_cli < 0){
      perror("Falló el 'accept'");
      exit(EXIT_FAILURE);
    }
    // Se crea un hilo para establecer la comunicacion
    pthread_create(&clientes[contador], NULL, atender, (void *) sock_cli);
    contador ++;
  }

  return(EXIT_SUCCESS);
}
