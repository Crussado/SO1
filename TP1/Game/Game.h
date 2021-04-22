#ifndef GAME_TYPES
#define GAME_TYPES
#include "Board/Board.h"

/******************************************************************************/
/* Representamos las células vivas como 'O' y las muertas como 'X' */

/******************************************************************************/
struct _game {
    board_t tablero;
    int generaciones; 
};

typedef struct _game game_t;
/******************************************************************************/

/* Cargamos el juego desde un archivo */
game_t *loadGame(const char *filename);

/* Guardamos el tablero 'board' en el archivo 'filename' */
void writeBoard(board_t board, const char *filename);

void play(game_t * game);

void endGame(game_t * game, const char *filename);

void gameDestroy(game_t * game);

/* Simulamos el Juego de la Vida de Conway con tablero 'board' la cantidad de
ciclos indicados en 'cycles' en 'nuprocs' unidades de procesamiento*/
board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc);

#endif
