#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

typedef struct Queen {
    Piece piece;
} Queen;

Queen* Queen_New(PieceId, Side, Position);
Queen* Queen_Clone(Queen*);
void Queen_Free(Piece*);

#endif