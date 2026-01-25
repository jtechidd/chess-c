#ifndef BISHOP_H
#define BISHOP_H

#include "piece.h"

#define BISHOP_TOTAL_DIRECTIONS 4
extern const Vector2 BISHOP_DIRECTIONS[];

typedef struct Bishop {
    Piece piece;
} Bishop;

Vector2* bishop_get_directions(size_t*);
Bishop* bishop_new(PieceId, Side, Vector2);
Bishop* bishop_clone(Bishop*);
Bishop* bishop_cast(Piece*);
MoveArray* bishop_get_positional_moves(Board*, Piece*);
void bishop_free(Piece*);

#endif