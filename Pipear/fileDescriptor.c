#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include<wait.h>

int main(int argc, char* argv[]) {
    char buff[1000];
    int fd[2];
    int res;
    pipe(fd);
    pid_t p = fork();
    if(p == 0) { //child fd 0 fd 1 prog1
        char *args[] = {argv[1], NULL};
        close(fd[0]);
        dup2(fd[1], STDOUT_FILENO);
        res = execv(argv[1], args);
    }
    else { //father fd 0 fd 1 prog2
        int status;
        wait(&status);
        if(WIFEXITED(status)){
            char *args[] = {argv[2], NULL};
            dup2(fd[0], STDIN_FILENO);
            close(fd[1]);
            res = execv(argv[2], args);
        }
    }
    return 0;
}
