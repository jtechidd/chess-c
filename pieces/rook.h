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
MoveArray* rook_get_positional_moves(Board*, Piece*);
void rook_free(Piece*);

#endif