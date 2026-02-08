#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

typedef struct knight_t {
  piece_t piece;
} knight_t;

piece_new_fn knight_piece_new;
int knight_piece_cast(knight_t **, piece_t *);

board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_knight;

#endif