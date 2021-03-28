#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <wait.h>
#include <stdlib.h>

// void agarrarArg(char* buff[], char** args[]) {
//     char delim[2] = " ";
//     *args[0] = strtok(buff, delim); //ruta 
//     for (int i = 0; *args[i] != NULL; i++) {
//         *args[i + 1] = strtok(NULL, delim); //demas arg.
//     }
// }

// void llamada(char* buff[], int* esperar) {
//     printf("PROMPT$ ");
//     fgets(buff, 50, stdin);
//     int len = strlen(buff);

//     if(buff[len - 2] == '&') {
//         esperar = 0;
//         buff[len - 3] = '\0';
//     }
//     else {
//         esperar = 1;
//         buff[len - 1] = '\0';
//     }
// }

void terminal() {
    char buff[100];
    int res = 0;
    pid_t p;
    int status = 0;
    char* args[10];
    char delim[2] = " ";
    int esperar = 1;

    printf("PROMPT$ ");
    fgets(buff, 50, stdin);
    int len = strlen(buff);

    if(buff[len - 2] == '&') {
        esperar = 0;
        buff[len - 3] = '\0';
    }
    else {
        esperar = 1;
        buff[len - 1] = '\0';
    }

    while(strcmp(buff, "exit") != 0) {
        p = fork();
        if (p < 0) {
            perror("Error al forkear!");
            return;
        }

        if (p == 0) {
            printf("%s", buff);
            args[0] = strtok(buff, delim); // ruta 
            for (int i = 0; args[i] != NULL; i++)
                args[i + 1] = strtok(NULL, delim); //demas arg.
            res = execv(args[0], args);
            if( res < 0)
                perror("Error al execvear!");
            exit(-1);
        }

        if(esperar) {
            wait(&status);
            
            printf("PROMPT$ ");
            fgets(buff, 50, stdin);
            len = strlen(buff);

            if(buff[len - 2] == '&') {
                esperar = 0;
                buff[len - 3] = '\0';
            }
            else {
                esperar = 1;
                buff[len - 1] = '\0';
            }

            if (!WIFEXITED(status))
                perror("Error retornar el nene!");
       }
        else {
            printf("PROMPT$ ");
            fgets(buff, 50, stdin);
            len = strlen(buff);

            if(buff[len - 2] == '&') {
                esperar = 0;
                buff[len - 3] = '\0';
            }
            else {
                esperar = 1;
                buff[len - 1] = '\0';
            }
        }
    }
}

int main(int argc, char**argv) {
    terminal();
    return 0;
}
