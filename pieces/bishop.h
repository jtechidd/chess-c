#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

typedef struct bishop_t {
  piece_t piece;
} bishop_t;

bishop_t *bishop_new(piece_id_t, side_t, vector2_t);
bishop_t *bishop_clone(bishop_t *);

uint8_t board_is_position_get_attacked_by_bishop(board_t *, side_t, vector2_t);

#endif