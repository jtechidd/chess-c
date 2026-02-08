#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

typedef struct queen_t {
  piece_t piece;
} queen_t;

piece_new_fn queen_piece_new;
int queen_piece_cast(queen_t **, piece_t *);

board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_queen;

#endif