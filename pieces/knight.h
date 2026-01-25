#ifndef KNIGHT_H
#define KNIGHT_H

#include "piece.h"

#define KNIGHT_TOTAL_DIRECTIONS 8
extern const Vector2 KNIGHT_DIRECTIONS[];

typedef struct Knight {
    Piece piece;
} Knight;

Knight* knight_new(PieceId, Side, Vector2);
Knight* knight_clone(Knight*);
Knight* knight_cast(Piece*);
MoveArray* knight_get_positional_moves(Board*, Piece*);
void knight_free(Piece*);

#endif