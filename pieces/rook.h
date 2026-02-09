#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

typedef struct rook_t {
  piece_t piece;
} rook_t;

#define WUR __attribute__((warn_unused_result()))

WUR piece_new_fn rook_piece_new;
WUR int rook_piece_cast(rook_t **, piece_t *);

WUR board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_rook;

#undef WUR

#endif