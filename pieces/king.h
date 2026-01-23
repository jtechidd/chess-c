#ifndef KING_H
#define KING_H

#include "piece.h"

typedef struct King {
    Piece piece;
    unsigned int has_been_moved : 1;
} King;

King* King_New(PieceId, Side, Position);
King* King_Clone(King*);

MoveArray* King_GetPositionalMoves(Board*, Piece*);
void King_Free(Piece*);

#endif