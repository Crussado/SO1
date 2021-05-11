struct SpipeBi {
   int fd1[2];
   int fd2[2];
};

typedef struct SpipeBi pipeBi;
#define BUFFER 1024

pipeBi* crear_PipeBi();

void setear(pipeBi* tuberia, pid_t p);

void enviar (pipeBi* tuberia, char* mensaje, pid_t p);

void recibir (pipeBi* tuberia, char* buffer, pid_t p);

void destruir(pipeBi* tuberia, pid_t p);
