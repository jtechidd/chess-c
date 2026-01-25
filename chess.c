#include "chess.h"

#include <stdlib.h>
#include <string.h>

Chess* Chess_New() {
    Chess* chess = (Chess*)malloc(sizeof(Chess));
    chess->turn = SIDE_WHITE;
    chess->board = board_new();
    return chess;
}

void Chess_Free(Chess* c) {
    board_free(c->board);
    free(c);
}
