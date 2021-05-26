#include <stdio.h>
#include <unistd.h> /* Read/Write + Fork */

int main() {
    char buff[100];
    // write(STDIN_FILENO, "holi\0", 5);
    // lseek(STDIN_FILENO, 0, SEEK_SET);
    // read(STDIN_FILENO, buff, 100);
    scanf("%s", buff);
    printf("prog 2 %s\n", buff);
    return 0;
}