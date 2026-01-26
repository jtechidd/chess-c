#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

#define QUEEN_TOTAL_DIRECTIONS 8
extern const vector2_t QUEEN_DIRECTIONS[];

typedef struct queen_t {
    piece_t piece;
} queen_t;

queen_t* queen_new(piece_id_t, side_t, vector2_t);
queen_t* queen_clone(queen_t*);
queen_t* queen_cast(piece_t*);
move_array_t* queen_get_positional_moves(piece_t*, board_t*);
void queen_free(piece_t*);

uint8_t board_is_position_get_attacked_by_queen(board_t*, side_t, vector2_t);

#endif