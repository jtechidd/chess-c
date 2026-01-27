#ifndef KING_H
#define KING_H

#include "piece.h"

typedef struct king_t {
  piece_t piece;
} king_t;

king_t *king_new(piece_id_t, side_t, vector2_t);
king_t *king_clone(king_t *);
king_t *king_cast(piece_t *);
move_array_t *king_get_moves(piece_t *, board_t *);
void king_castle(piece_t *, move_t *, board_t *);
void king_free(piece_t *);

uint8_t board_is_position_get_attacked_by_king(board_t *, side_t, vector2_t);

#endif