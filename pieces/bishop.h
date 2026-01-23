#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

typedef struct Bishop {
    Piece piece;
} Bishop;

Bishop* Bishop_New(PieceId, Side, Position);
Bishop* Bishop_Clone(Bishop*);

MoveArray* Bishop_GetPositionalMoves(Board*, Piece*);
void Bishop_Free(Piece*);

#endif