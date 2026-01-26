#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

#define PAWN_UP_TOTAL_DIRECTIONS 2
extern const vector2_t PAWN_UP_DIRECTIONS[];
#define PAWN_TAKE_TOTAL_DIRECTIONS 2
extern const vector2_t PAWN_TAKE_DIRECTIONS[];

typedef struct pawn_t {
    piece_t piece;
} pawn_t;

pawn_t* pawn_new(piece_id_t, side_t, vector2_t);
pawn_t* pawn_clone(pawn_t*);
pawn_t* pawn_cast(piece_t*);
move_array_t* pawn_get_positional_moves(piece_t*, board_t*);
void pawn_free(piece_t*);

uint8_t board_is_position_get_attacked_by_pawn(board_t*, side_t, vector2_t);

#endif