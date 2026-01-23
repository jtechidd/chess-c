#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

typedef struct Knight {
    Piece piece;
} Knight;

Knight* Knight_New(PieceId, Side, Position);
Knight* Knight_Clone(Knight*);

MoveArray* Knight_GetPositionalMoves(Board*, Piece*);
void Knight_Free(Piece*);

#endif