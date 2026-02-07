#ifndef ROOK_H
#define ROOK_H

#include "piece.h"

typedef struct rook_t {
  piece_t piece;
} rook_t;

rook_t *rook_new(piece_id_t, side_t, vector2_t);
rook_t *rook_clone(rook_t *);
rook_t *rook_cast(piece_t *);

board_is_position_being_attacked_by_piece_t
    board_is_position_being_attacked_by_rook;

#endif