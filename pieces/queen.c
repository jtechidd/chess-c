#include "queen.h"

#include <stdlib.h>

Queen* Queen_New(PieceId piece_id, Side side, Position position) {
    Queen* queen = (Queen*)malloc(sizeof(Queen));

    // Set piece fields
    queen->piece.id = piece_id;
    queen->piece.side = side;
    queen->piece.type = PieceType_Queen;
    queen->piece.position = position;
    queen->piece.is_captured = 0;

    // Set functions
    queen->piece.Piece_GetPositionalMoves = Queen_GetPositionalMoves;
    queen->piece.Piece_Free = Queen_Free;

    return queen;
}

Queen* Queen_Clone(Queen* queen_src) {
    Queen* queen = Queen_New(queen_src->piece.id, queen_src->piece.side, queen_src->piece.position);

    // Set piece fields
    queen->piece.is_captured = queen_src->piece.is_captured;

    return queen;
}

MoveArray* Queen_GetPositionalMoves(Board* board, Piece* piece) {
    Queen* queen = (Queen*)piece;
    // TODO: Find queen positional moves on board
}

void Queen_Free(Piece* piece) {
    Queen* queen = (Queen*)piece;
    free(queen);
}
