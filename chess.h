#ifndef CHESS_H
#define CHESS_H

#include "board.h"

typedef struct chess_t {
    side_t turn;
    board_t* board;
} chess_t;

chess_t* Chess_New();
void Chess_Free(chess_t*);

#endif