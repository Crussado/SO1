#include <pthread.h> /* PThread */

/* Definición de la estructura y sinónimo de tipo.*/
struct cond_barrier {
    pthread_cond_t *variable;
    pthread_mutex_t *mutex;
    pthread_mutex_t *recursoContador;
    int n;
    int waiting;
};

typedef struct cond_barrier barrier_t;
/************/

/************/
/* Operaciones*/

/* Creación de una barrera de condición, tomando como argumento la cantidad de
hilos que se van a esperar*/
int barrier_init(barrier_t *barr, unsigned int count);

/* Función *bloqueante* para esperar a los demás hilos */
int barrier_wait(barrier_t *barr);

/* Eliminación de la barrera */
int barrier_destroy(barrier_t *barr);
