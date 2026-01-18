#include "chess.h"

#include <stdlib.h>
#include <string.h>

Chess* Chess_New() {
    Chess* chess = (Chess*)malloc(sizeof(Chess));
    chess->turn = Side_White;
    chess->board = Board_New();
    return chess;
}

void Chess_Free(Chess* c) {
    Board_Free(c->board);
    free(c);
}
