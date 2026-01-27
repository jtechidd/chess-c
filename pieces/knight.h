#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

typedef struct knight_t {
  piece_t piece;
} knight_t;

knight_t *knight_new(piece_id_t, side_t, vector2_t);
knight_t *knight_clone(knight_t *);
knight_t *knight_cast(piece_t *);
move_array_t *knight_get_moves(piece_t *, board_t *);
void knight_free(piece_t *);

uint8_t board_is_position_get_attacked_by_knight(board_t *, side_t, vector2_t);

#endif