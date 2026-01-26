#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

#define BISHOP_TOTAL_DIRECTIONS 4
extern const vector2_t BISHOP_DIRECTIONS[];

typedef struct bishop_t {
    piece_t piece;
} bishop_t;

vector2_t* bishop_get_directions(size_t*);
bishop_t* bishop_new(piece_id_t, side_t, vector2_t);
bishop_t* bishop_clone(bishop_t*);
bishop_t* bishop_cast(piece_t*);
move_array_t* bishop_get_positional_moves(piece_t*, board_t*);
void bishop_free(piece_t*);

uint8_t board_is_position_get_attacked_by_bishop(board_t*, side_t, vector2_t);

#endif