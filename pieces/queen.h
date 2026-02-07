#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

typedef struct queen_t {
  piece_t piece;
} queen_t;

queen_t *queen_new(piece_id_t, side_t, vector2_t);
queen_t *queen_clone(queen_t *);

board_is_position_being_attacked_by_piece_fn
    board_is_position_being_attacked_by_queen;

#endif