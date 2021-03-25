#include<stdio.h>
#include<unistd.h>
#include<wait.h>
#include<stdlib.h>

int main(int argc, char**argv) {
    char buff[100];
    int res = 0;
    printf("PROMPT$ ");
    scanf("%s", buff);
    // "/usr/bin/ls" = buff
    printf("%s", buff);
    char* args[] = { buff, NULL};
    
    res = execv(buff, args);

    if( res < 0){
        perror("Algo pasó!");
    }

    return 0;
}
