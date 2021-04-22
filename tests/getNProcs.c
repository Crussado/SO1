#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/sysinfo.h>

void * f(void* arg) {
    int i = get_nprocs();
    printf("3: %d\n", i);
    sleep(3);
}

int main() {
    int i = get_nprocs();
    int j = get_nprocs_conf();
    printf("1: %d\n", j);
    sleep(2);
    i = get_nprocs();
    printf("2: %d\n", i);
    
    pthread_t hilo;
    pthread_create(&hilo, NULL, f, NULL);

    i = get_nprocs();
    printf("4: %d\n", i);
    pthread_join(hilo, NULL);
    int child = fork();
    i = get_nprocs();
    printf("5: %d\n", i);

    return 0;
}