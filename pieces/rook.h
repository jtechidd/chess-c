#ifndef ROOK_H
#define ROOK_H

#include <stdlib.h>

#include "piece.h"

#define ROOK_TOTAL_DIRECTIONS 4
extern const vector2_t ROOK_DIRECTIONS[];

typedef struct rook_t {
    piece_t piece;
} rook_t;

rook_t* rook_new(piece_id_t, side_t, vector2_t);
rook_t* rook_clone(rook_t*);
rook_t* rook_cast(piece_t*);
move_array_t* rook_get_positional_moves(piece_t*, board_t*);
void rook_free(piece_t*);

uint8_t board_is_position_get_attacked_by_rook(board_t*, side_t, vector2_t);

#endif