#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

typedef struct bishop_t {
  piece_t piece;
} bishop_t;

#define WUR __attribute__((warn_unused_result()))

WUR piece_new_fn bishop_piece_new;
WUR int bishop_piece_cast(bishop_t **, piece_t *);

WUR board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_bishop;

#undef WUR

#endif