#ifndef KING_H
#define KING_H

#include "piece.h"

typedef struct King {
    Piece piece;
    unsigned int has_been_moved : 1;
} King;

King* king_new(PieceId, Side, Vector2);
King* king_clone(King*);

MoveArray* king_get_positional_moves(Board*, Piece*);
void king_free(Piece*);

#endif