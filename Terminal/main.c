#include <stdio.h>
#include <unistd.h>
#include <wait.h>
#include <stdlib.h>
#include <string.h>

void terminal() {

    char buff[100];
    int res = 0;
    pid_t p;
    printf("PROMPT$ ");
    scanf("%s", buff);
    int status = 0;
    char* args[2];
    while(strcmp(buff, "exit") != 0) {
        p = fork();
        if (p < 0) {
            perror("Error al forkear!");
            return;
        }
        if (p == 0) {
            args[0] = buff;
            args[1] = NULL;
            res = execv(buff, args);
            if( res < 0)
                perror("Error al execvear!");
            exit(-1);
        }
        wait(&status);
        if (WIFEXITED(status)) {
            printf("PROMPT$ ");
            scanf("%s", buff);
        } 
        else 
            perror("Error retornar el nene!");
        
    }
}

int main(int argc, char**argv) {
    terminal();
    return 0;
}
