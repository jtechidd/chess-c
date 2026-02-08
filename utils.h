#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>

#include "enums.h"
#include "vector2.h"

bool is_piece_id_valid(piece_id_t);
bool is_position_in_bound(vector2_t);
bool is_position_top(vector2_t);
bool is_position_bottom(vector2_t);
bool is_side_valid(side_t);
bool is_opposite_side(side_t, side_t);

#endif