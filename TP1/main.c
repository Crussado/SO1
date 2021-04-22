#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "Game/Game.h"

int main(int argc, char* argv[]) {
    if(argc != 2) {
        perror("Error en la cantidad de argumentos.");
        exit(EXIT_FAILURE);
    }

    game_t * game = NULL;
    game = loadGame(argv[1]);

    if(game == NULL)
        return -1; 

    play(game);

    // Encontrar ruta para el guardado del final del juego
    char filename[strlen(argv[1]) + 2];
    sscanf(argv[1], "%[^.].game", filename);
    char resto[] = ".final";
    strcat(filename, resto);

    endGame(game, filename);

    return 0;
}