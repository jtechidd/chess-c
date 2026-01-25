#include "king.h"

#include <stdlib.h>

King* king_new(PieceId piece_id, Side side, Vector2 position) {
    King* king = (King*)malloc(sizeof(King));

    // Set piece fields
    king->piece.id = piece_id;
    king->piece.side = side;
    king->piece.type = PIECE_TYPE_KING;
    king->piece.position = position;
    king->piece.is_captured = 0;

    // Set functions
    king->piece.piece_get_positional_moves = king_get_positional_moves;
    king->piece.piece_free = king_free;

    // Set fields
    king->has_been_moved = 0;

    return king;
}

King* king_clone(King* king_src) {
    King* king = king_new(king_src->piece.id, king_src->piece.side, king_src->piece.position);

    // Set piece fields
    king->piece.is_captured = king_src->piece.is_captured;

    // Set fields
    king->has_been_moved = king_src->has_been_moved;

    return king;
}

MoveArray* king_get_positional_moves(Board* board, Piece* piece) {
    King* king = (King*)piece;

    MoveArray* move_array = move_array_new();

    // TODO: Find king positional moves on board
    return move_array;
}

void king_free(Piece* piece) {
    King* king = (King*)piece;
    free(king);
}
