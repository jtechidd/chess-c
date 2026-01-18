#include "bishop.h"

#include <stdlib.h>

Bishop* Bishop_New(PieceId piece_id, Side side, Position position) {
    Bishop* bishop = (Bishop*)malloc(sizeof(Bishop));

    // Set piece fields
    bishop->piece.id = piece_id;
    bishop->piece.side = side;
    bishop->piece.type = PieceType_Bishop;
    bishop->piece.position = position;
    bishop->piece.is_captured = 0;

    // Set functions
    bishop->piece.Piece_Free = Bishop_Free;

    return bishop;
}

Bishop* Bishop_Clone(Bishop* bishop_src) {
    Bishop* bishop = Bishop_New(bishop_src->piece.id, bishop_src->piece.side, bishop_src->piece.position);

    // Set piece fields
    bishop->piece.is_captured = bishop_src->piece.is_captured;

    return bishop;
}

void Bishop_Free(Piece* piece) {
    Bishop* bishop = (Bishop*)piece;
    free(bishop);
}
