#ifndef QUEEN_H
#define QUEEN_H

#include "piece.h"

#define QUEEN_TOTAL_DIRECTIONS 8
extern const Vector2 QUEEN_DIRECTIONS[];

typedef struct Queen {
    Piece piece;
} Queen;

Queen* queen_new(PieceId, Side, Vector2);
Queen* queen_clone(Queen*);
Queen* queen_cast(Piece*);
MoveArray* queen_get_positional_moves(Piece*, Board*);
void queen_free(Piece*);

uint8_t board_is_position_get_attacked_by_queen(Board*, Side, Vector2);

#endif