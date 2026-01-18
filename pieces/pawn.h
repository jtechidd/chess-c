#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

typedef struct Pawn {
    Piece piece;
    unsigned int has_been_moved : 1;
} Pawn;

Pawn* Pawn_New(PieceId, Side, Position);
Pawn* Pawn_Clone(Pawn*);
void Pawn_Free(Piece*);

#endif