#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

typedef struct Queen {
    Piece piece;
} Queen;

Queen* queen_new(PieceId, Side, Vector2);
Queen* queen_clone(Queen*);

MoveArray* queen_getposmoves(Board*, Piece*);
void queen_free(Piece*);

#endif