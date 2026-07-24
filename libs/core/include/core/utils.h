#ifndef CH_UTILS_H
#define CH_UTILS_H

#include "core/common.h"
#include "core/vector2.h"

bool ch_is_valid_file(char file);
bool ch_is_valid_rank(char rank);
int8_t ch_rank_to_vertical_position(char rank);
int8_t ch_file_to_horizontal_position(char file);
ch_piece_type_t ch_char_to_piece_type(char piece_char);
char ch_piece_type_to_char(ch_piece_type_t type);
bool ch_is_position_in_bound(ch_vector2_t pos);

#endif