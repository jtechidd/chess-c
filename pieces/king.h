#ifndef KING_H
#define KING_H

#include "piece.h"

typedef struct move_t move_t;

typedef struct king_t {
  piece_t piece;
} king_t;

#define WUR __attribute__((warn_unused_result()))

WUR piece_new_fn king_piece_new;
WUR int king_piece_cast(king_t **, piece_t *);
WUR int king_piece_castle(piece_t *, move_t *, board_t *);

WUR board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_king;

#undef WUR

#endif