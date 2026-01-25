#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

#define PAWN_MOVE_UP_TOTAL_DIRECTIONS 2
extern const Vector2 PAWN_MOVE_UP_DIRECTIONS[];
#define PAWN_MOVE_TAKE_TOTAL_DIRECTIONS 2
extern const Vector2 PAWN_MOVE_TAKE_DIRECTIONS[];

typedef struct Pawn {
    Piece piece;
    unsigned int has_been_moved : 1;
    unsigned int first_time_moved : 1;
} Pawn;

Pawn* pawn_new(PieceId, Side, Vector2);
Pawn* pawn_clone(Pawn*);
Pawn* pawn_cast(Piece*);
MoveArray* pawn_get_positional_moves(Board*, Piece*);
void pawn_free(Piece*);

#endif