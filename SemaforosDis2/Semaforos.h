#include <pthread.h> /* PThread */
#include <semaphore.h>

struct semaphore_d {
    sem_t sem;
    int contador;
    pthread_mutex_t recursoContador;
};

typedef struct semaphore_d sem_d;

/* Función de creación de Semáforo */
int semd_init(sem_d *sem, int init);

/* Incremento del semáforo. */
int semd_incr(sem_d *sem);

/* Decremento del semáforo. Notar que es una función que puede llegar a bloquear
   el proceso.*/
int semd_decr(sem_d *sem);

/* Destrucción de un semáforo */
int semd_destroy(sem_d *sem);
