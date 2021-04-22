#include <pthread.h> /* PThread */
#include <stdio.h> /* Printf */
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include "SList/Slist.h"
#include "Board.h"

// board_init crea un tablero vacio
int board_init(board_t *board, size_t col, size_t row) {
    board->col = col;
    board->row = row;
    board->analizar = slist_crear();

    //Pedimos memoria para crear el tablero.
    board->tablero = malloc(sizeof(char*) * row);
    int rowNew = (int) row;
    for(int i = 0; i < rowNew; i ++)
        board->tablero[i] = malloc(sizeof(char) * col);

    return 0;
}

// board_set setea en el tablero una determinada celula con determinado estado
int board_set(board_t board, unsigned int col, unsigned int row, char val) {
    board.tablero[row][col] = val;
    return 0;
}

// board_get obtiene el estado de una celula
char board_get(board_t board, unsigned int col, unsigned int row) {
    return board.tablero[row][col];
}

char board_get_round(board_t board, int col, int row) {
    return board.tablero[(row + board.row) % board.row][(col + board.col) % board.col];
}

char board_set_round(board_t board, int col, int row, char val) {
    board.tablero[(row + board.row) % board.row][(col + board.col) % board.col] = val;
    return 0;
}


int board_init_def(board_t *board, size_t col, size_t row, char def) {
    board->col = col;
    board->row = row;
    board->analizar = slist_crear();

    //Pedimos memoria para crear el tablero.
    board->tablero = malloc(sizeof(char*) * row);
    int rowNew = (int) row;
    int colNew = (int) col;
    for(int i = 0; i < rowNew; i ++)
        board->tablero[i] = malloc(sizeof(char) * col);

    for(int i = 0; i < rowNew; i ++)
        for(int j = 0; j < colNew; j ++)
            board_set(*board, j, i, def);

    return 0;
}   

// arreglar_coord arregla las coordenadas de forma "circular"
// respecto a un determinado tablero
void arreglar_coord(board_t *board, int *x, int *y) {
    *x = (*x + board->row) % board->row;
    *y = (*y + board->col) % board->col;
}

// agrega a la slista del tablero todas las celulas vecinas respecto
// a la celula en el casillero (x, y)
void agregar_a_la_redonda(board_t *board, int x, int y) {
    int xN, yN;
    for(int i = x - 1; i <= x + 1; i++) {
        xN = i;
        for(int j = y - 1; j <= y + 1; j++) {
            yN = j;
            arreglar_coord(board, &xN, &yN);
            board->analizar = slist_agregar_final(board->analizar, xN, yN, board_get(*board, yN, xN));
        }
    }
}
// board_load "vuelca" la configuracion de un tablero brindada por un str
// en el tablero
int board_load(board_t *board, char *str) {
    int num, fila = 0, col = 0;
    int rowNew = (int) board->row, colNew = (int) board->col;
    char estado;
    while(fila < rowNew) {
        if (sscanf(str, "%d%c%[^-]", &num, &estado, str) == 3) {
            for(int i = 0; i < num; i++) {
                board_set(*board, col + i, fila, estado);
            }
            col += num;
        }

        if(str[0] == '\n') {
            str ++;
            fila ++;
            col = 0;
        }
    }
    // Se guardan los casilleros relevantes a analizar para la sig
    // generacion
    for(int i = 0; i < rowNew; i++)
        for(int j = 0; j < colNew; j++)
            if(board_get(*board, j, i) == ALIVE)
                agregar_a_la_redonda(board, i, j);
    return 0;
}

// board_show guarda la informacion de un tablero en un string.
void board_show(board_t board, char * res) {
    int contador = 0;
    char estado, aux[10];
    int rowNew = (int) board.row, colNew = (int) board.col;
    for(int i = 0; i < rowNew; i++) {
        contador = 0;
        estado = board.tablero[i][0];
        for(int j = 0; j < colNew; j++) {
            if(board_get(board, j, i) != estado) {
                sprintf(aux, "%d%c", contador, estado);
                strcat(res, aux);
                estado = board_get(board, j, i);
                contador = 1;
            }
            else
                contador ++;
        }
        sprintf(aux, "%d%c", contador, estado);
        strcat(res, aux);
        strcat(res, "\n");
    }
    strcat(res, "\0");
}

// writeBoard escribe la informacion de un tablero en un archivo
void writeBoard(board_t board, const char *filename) {
    char res[board.row * board.col * 2 + board.row + 1];
    board_show(board, res);

    FILE * fp = fopen(filename, "w");
    
    if(fp == NULL){
        perror("Error en la lectura");
        exit(EXIT_FAILURE);
    }

    fprintf(fp, "%s", res);
    fclose(fp);
}

// board_destroy destruye un tablero
void board_destroy(board_t *board) {
    int rowNew = (int) board->row;
    for(int i = 0; i < rowNew; i ++)
        free(board->tablero[i]);
    slist_destruir(board->analizar);
    free(board->tablero);
}

// board_print imprime el tablero
void board_print(board_t *board) {
    int rowNew = (int) board->row;
    int colNew = (int) board->col;
    for(int i = 0; i < rowNew; i++) {
        for(int j = 0; j < colNew; j++)
            printf("%c", board_get(*board, j, i));
        printf("\n");
    } 
}

// la funcion analizar toma el tablero y una celula, y analiza que 
// estado tendra en la siguiente generacion
char analizar(board_t *board, int x, int y, char state) {
    int xN, yN, cont = 0;

    for(int i = x - 1; i <= x + 1; i++) {
        xN = i;
        for(int j = y - 1; j <= y + 1; j++) {
            yN = j;
            arreglar_coord(board, &xN, &yN);
            if(xN != x || yN != y)
                if(board_get(*board, yN, xN) == ALIVE)
                    cont ++;
        }
    }
    if(state == DEAD && cont == 3)
        return ALIVE;
    if(state == ALIVE && (cont == 2 || cont == 3))
        return ALIVE;
    return DEAD;
}

// aux es una funcion auxiliar de acomodar que protege y analiza la zona critica
int aux(board_t * board, int * x, int * y, char * state, pthread_mutex_t *candado) {
    pthread_mutex_lock(candado);
    if(slist_vacia(board->analizar)){
        pthread_mutex_unlock(candado);
        return 0;
    }
    board->analizar = pop(board->analizar, x, y, state);
    *state = board_get(*board, *y, *x); // corroboro con la info del tablero
    pthread_mutex_unlock(candado);
    return 1;
}

// estructura de argumentos para los hilos
struct arg {
    int i;
    int cycles;
    board_t *board;
    pthread_barrier_t *barrier1;
    pthread_barrier_t *barrier2;
    pthread_mutex_t *candado;
};

// acomodar son los hilos que setean las celulas en el tablero
void * acomodar(void* arg) {
    struct arg * argumentos = (struct arg *) arg;
    int x, y;
    char state, newState;
    SList cambios = slist_crear();

    for(int i = 0; i < argumentos->cycles; i++) {
        // se ve como cambian todas las celulas relevantes
        // para la proxima generacion
        while(aux(argumentos->board, &x, &y, &state, argumentos->candado)) {
            newState = analizar(argumentos->board, x, y, state);
            if(!(newState == state && state == DEAD))
                cambios = push(cambios, x, y, newState);
        }

        pthread_barrier_wait(argumentos->barrier1);

        // se altera el tablero en base al analisis anterior
        while (!slist_vacia(cambios)) {
            cambios = pop(cambios, &x, &y, &state);
            board_set(*argumentos->board, y, x, state);
            // se vuelve a encontrar todas las celulas para anlizar
            if(state == ALIVE) {
                pthread_mutex_lock(argumentos->candado);
                agregar_a_la_redonda(argumentos->board, x, y);
                pthread_mutex_unlock(argumentos->candado);
            }
        }

        pthread_barrier_wait(argumentos->barrier2);

    }

    slist_destruir(cambios);

    pthread_exit(EXIT_SUCCESS);
}

// congwayGol modifica el tablero una cierta cantidad de ciclos.
board_t *congwayGoL(board_t *board, unsigned int cycles, const int nuproc) {
    pthread_t acomodadores[nuproc];
    struct  arg argumentos[nuproc];
    pthread_barrier_t barrier1;
    pthread_barrier_init(&barrier1, NULL, nuproc);
    pthread_barrier_t barrier2;
    pthread_barrier_init(&barrier2, NULL, nuproc);
    pthread_mutex_t candado = PTHREAD_MUTEX_INITIALIZER;

    for(int i = 0; i < nuproc; i ++){
        argumentos[i].barrier1 = &barrier1;
        argumentos[i].barrier2 = &barrier2;
        argumentos[i].i = i;
        argumentos[i].cycles = cycles;
        argumentos[i].board = board;
        argumentos[i].candado = &candado;
        pthread_create(&acomodadores[i], NULL, acomodar, (void*) &argumentos[i]);
    }

    for(int i = 0; i < nuproc; i ++)
        pthread_join(acomodadores[i], NULL);

    pthread_barrier_destroy(&barrier1);
    pthread_barrier_destroy(&barrier2);
    pthread_mutex_destroy(&candado);

    return board;

}

