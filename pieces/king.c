#include "king.h"

#include <stdlib.h>

King* King_New(PieceId piece_id, Side side, Position position) {
    King* king = (King*)malloc(sizeof(King));

    // Set piece fields
    king->piece.id = piece_id;
    king->piece.side = side;
    king->piece.type = PieceType_King;
    king->piece.position = position;
    king->piece.is_captured = 0;

    // Set functions
    king->piece.Piece_Free = King_Free;

    // Set fields
    king->has_been_moved = 0;

    return king;
}

King* King_Clone(King* king_src) {
    King* king = King_New(king_src->piece.id, king_src->piece.side, king_src->piece.position);

    // Set piece fields
    king->piece.is_captured = king_src->piece.is_captured;

    // Set fields
    king->has_been_moved = king_src->has_been_moved;

    return king;
}

void King_Free(Piece* p) {
    King* king = (King*)p;
    free(king);
}
