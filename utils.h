#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include "enums.h"
#include "vector2.h"

uint8_t is_piece_id_valid(piece_id_t);
uint8_t is_side_valid(side_t);
uint8_t is_position_in_boundary(vector2_t);
uint8_t is_position_top(vector2_t);
uint8_t is_position_bottom(vector2_t);
uint8_t is_opposite_side(side_t, side_t);

#endif