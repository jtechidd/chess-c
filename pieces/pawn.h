#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

#define PAWN_UP_TOTAL_DIRECTIONS 2
extern const Vector2 PAWN_UP_DIRECTIONS[];
#define PAWN_TAKE_TOTAL_DIRECTIONS 2
extern const Vector2 PAWN_TAKE_DIRECTIONS[];

typedef struct Pawn {
    Piece piece;
    unsigned int has_been_moved : 1;
    unsigned int first_time_moved : 1;
} Pawn;

Pawn* pawn_new(PieceId, Side, Vector2);
Pawn* pawn_clone(Pawn*);
Pawn* pawn_cast(Piece*);
MoveArray* pawn_get_positional_moves(Piece*, Board*);
void pawn_free(Piece*);

uint8_t board_is_position_get_attacked_by_pawn(Board*, Side, Vector2);

#endif