#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

void agarrarArg(char* buff, char* * args[]) {
    char delim[2] = " ";
    *args[0] = strtok(buff, delim); //ruta 
    for (int i = 0; *args[i] != NULL; i++) {
        *args[i + 1] = strtok(NULL, delim); //demas arg.
    }
}

void terminal() {
    char buff[100];
    int res = 0;
    pid_t p;
    int status = 0;
    char* args[10];
    char delim[2] = " ";

    printf("PROMPT$ ");
    fgets(buff, 50, stdin);
    int len = strlen(buff);
    buff[len - 1] = '\0';
    while(strcmp(buff, "exit") != 0) {
        p = fork();
        if (p < 0) {
            perror("Error al forkear!");
            return;
        }
        if (p == 0) {
            printf("%s", buff);
            args[0] = strtok(buff, delim); // ruta 
            for (int i = 0; args[i] != NULL; i++) {
                args[i + 1] = strtok(NULL, delim); //demas arg.
            }
            res = execv(args[0], args);
            if( res < 0)
                perror("Error al execvear!");
            exit(1);
        }
        wait(&status); // pid = 5
        if (WIFEXITED(status)) {
            printf("PROMPT$ ");
            fgets(buff, 50, stdin);
            len = strlen(buff);
            buff[len - 1] = '\0';
        } 
        else
            perror("Error retornar el nene!");
    }
}

int main(int argc, char**argv) {
    terminal();
    return 0;
}
