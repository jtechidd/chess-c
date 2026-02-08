#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

typedef struct bishop_t {
  piece_t piece;
} bishop_t;

piece_new_fn bishop_piece_new;
int bishop_piece_cast(bishop_t **, piece_t *);

board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_bishop;

#endif