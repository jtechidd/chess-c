#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

typedef struct bishop_t {
  piece_t piece;
} bishop_t;

piece_t *bishop_piece_new(piece_id_t, side_t, vector2_t);
bishop_t *bishop_piece_cast(piece_t *);

board_is_position_get_attacked_by_piece_fn
    board_is_position_get_attacked_by_bishop;

#endif