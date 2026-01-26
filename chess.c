#include "chess.h"

#include <stdlib.h>
#include <string.h>

chess_t* Chess_New() {
    chess_t* chess = (chess_t*)malloc(sizeof(chess_t));
    chess->turn = SIDE_WHITE;
    chess->board = board_new();
    return chess;
}

void Chess_Free(chess_t* c) {
    board_free(c->board);
    free(c);
}
