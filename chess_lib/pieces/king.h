#ifndef KING_H
#define KING_H

#include "piece.h"

typedef struct move_t move_t;

typedef struct king_t {
  piece_t piece;
} king_t;

piece_t *king_piece_new(piece_id_t, side_t, vector2_t);
king_t *king_piece_cast(piece_t *);
void king_piece_castle(piece_t *, move_t *, board_t *);

board_is_position_get_attacked_by_piece_fn
    board_is_position_being_attacked_by_king;

#endif