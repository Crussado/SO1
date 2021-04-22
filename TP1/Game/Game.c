#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <pthread.h> /* PThread */
#include <assert.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "Game.h"
#include "Board/Board.h"

// loadGame carga el juegod desde un fichero.
game_t *loadGame(const char *fileName) {
    FILE * fp = fopen(fileName, "r"); 
    
    if(fp == NULL) {
        perror("Error en la lectura");
        return NULL;
    }

    size_t row, col;
    
    game_t * juego = malloc(sizeof(game_t));

    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    fseek(fp, 0, SEEK_SET);  /*igual que rewind(f); */

    char config[fsize + 1];
    fread(config, 1, fsize, fp);
    config[fsize] = 0;
    
    fclose(fp);

    sscanf(config, "%d %ld %ld\n%[^-]", &juego->generaciones, &row, &col, config);

    board_init(&juego->tablero, col, row);

    board_load(&juego->tablero, config);

    return juego;

}

// Play realiza el avance de las generaciones en el juego
void play(game_t * game) {
    congwayGoL(&game->tablero, game->generaciones, get_nprocs());
}

// gameDestroy destruye un juego dado
void gameDestroy(game_t * game) {
    board_destroy(&game->tablero);
    free(game);
}

// endGame termina el juego guardando su informacion en un
// archivo y destruyendo la estructura
void endGame(game_t * game, const char *filename) {
    writeBoard(game->tablero, filename);
    gameDestroy(game);
}
