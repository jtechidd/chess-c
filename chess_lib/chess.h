#ifndef CHESS_H
#define CHESS_H

#include "enums.h"

typedef struct board_t board_t;

typedef struct chess_t {
  side_t turn;
  board_t *board;
} chess_t;

chess_t *chess_new();
void chess_free(chess_t *);

#endif