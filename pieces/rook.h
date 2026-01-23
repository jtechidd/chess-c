#ifndef ROOK_H
#define ROOK_H

#include <stdlib.h>

#include "piece.h"

typedef struct Rook {
    Piece piece;
    unsigned int has_been_moved : 1;
} Rook;

Rook* Rook_New(PieceId, Side, Position);
Rook* Rook_Clone(Rook*);

MoveArray* Rook_GetPositionalMoves(Board*, Piece*);
void Rook_Free(Piece*);

#endif