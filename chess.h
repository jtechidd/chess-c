#ifndef CHESS_H
#define CHESS_H

#include "board.h"

typedef struct Chess {
    Side turn;
    Board* board;
} Chess;

Chess* Chess_New();
void Chess_Free(Chess*);

#endif