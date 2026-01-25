#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#include "enums.h"
#include "vector2.h"

uint8_t is_piece_id_valid(PieceId);
uint8_t is_position_in_boundary(Vector2);
uint8_t is_position_top(Vector2);
uint8_t is_position_bottom(Vector2);
uint8_t is_opposite_side(Side, Side);

#endif