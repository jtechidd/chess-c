#ifndef ROOK_H
#define ROOK_H

#include <stdlib.h>

#include "piece.h"

#define ROOK_TOTAL_DIRECTIONS 4
extern const Vector2 ROOK_DIRECTIONS[];

typedef struct Rook {
    Piece piece;
    unsigned int has_been_moved : 1;
} Rook;

Rook* rook_new(PieceId, Side, Vector2);
Rook* rook_clone(Rook*);
Rook* rook_cast(Piece*);
MoveArray* rook_get_positional_moves(Piece*, Board*);
void rook_free(Piece*);

uint8_t board_is_position_get_attacked_by_rook(Board*, Side, Vector2);

#endif