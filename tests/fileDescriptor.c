#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/sysinfo.h>

int main(int argc, char* argv[]) {
    char buff[1000];
    int id = getpid();
    write(STDOUT_FILENO, "buff", 4);
    write(STDIN_FILENO, "buff", 4);
    lseek(STDIN_FILENO, -1, SEEK_SET);
    // dup2(STDOUT_FILENO, STDIN_FILENO);

    read(STDIN_FILENO, buff, 1000);

    printf("%s\n", buff);
    return 0;
}
