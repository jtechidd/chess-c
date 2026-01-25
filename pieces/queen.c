#include "queen.h"

#include <stdlib.h>

Queen* queen_new(PieceId piece_id, Side side, Vector2 position) {
    Queen* queen = (Queen*)malloc(sizeof(Queen));

    // Set piece fields
    queen->piece.id = piece_id;
    queen->piece.side = side;
    queen->piece.type = PIECE_TYPE_QUEEN;
    queen->piece.position = position;
    queen->piece.is_captured = 0;

    // Set functions
    queen->piece.piece_get_positional_moves = queen_getposmoves;
    queen->piece.piece_free = queen_free;

    return queen;
}

Queen* queen_clone(Queen* queen_src) {
    Queen* queen = queen_new(queen_src->piece.id, queen_src->piece.side, queen_src->piece.position);

    // Set piece fields
    queen->piece.is_captured = queen_src->piece.is_captured;

    return queen;
}

MoveArray* queen_getposmoves(Board* board, Piece* piece) {
    Queen* queen = (Queen*)piece;

    MoveArray* move_array = move_array_new();

    // TODO: Find queen positional moves on board
    return move_array;
}

void queen_free(Piece* piece) {
    Queen* queen = (Queen*)piece;
    free(queen);
}
