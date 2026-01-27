#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include "enums.h"
#include "vector2.h"

uint8_t is_piece_id_valid(piece_id_t);
uint8_t is_position_in_bound(vector2_t);
uint8_t is_position_top(vector2_t);
uint8_t is_position_bottom(vector2_t);
uint8_t is_opposite_side(side_t, side_t);
void get_all_piece_ids_range_by_side(side_t, piece_id_t *, piece_id_t *);
piece_id_t get_king_piece_id_by_side(side_t);

#endif