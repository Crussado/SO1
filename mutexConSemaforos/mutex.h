#include <semaphore.h>

struct Smutex {
    sem_t sem;
};

typedef struct Smutex mutex;

int mutex_init(mutex *m);

int mutex_lock(mutex *m);

int mutex_unlock(mutex *m);

int mutex_destroy(mutex *m);

