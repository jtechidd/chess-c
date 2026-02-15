#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

typedef struct knight_t {
  piece_t piece;
} knight_t;

piece_t *knight_piece_new(piece_id_t, side_t, vector2_t);
knight_t *knight_piece_cast(piece_t *);

board_is_position_get_attacked_by_piece_fn
    board_is_position_get_attacked_by_knight;

#endif