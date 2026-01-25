#ifndef KING_H
#define KING_H

#include "piece.h"

#define KING_TOTAL_DIRECTIONS 8
extern const Vector2 KING_DIRECTIONS[];

typedef struct King {
    Piece piece;
    unsigned int has_been_moved : 1;
} King;

King* king_new(PieceId, Side, Vector2);
King* king_clone(King*);
King* king_cast(Piece*);
MoveArray* king_get_positional_moves(Piece*, Board*);
void king_free(Piece*);

uint8_t board_is_position_get_attacked_by_king(Board*, Side, Vector2);

#endif