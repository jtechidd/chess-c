#include "rook.h"

Rook* Rook_New(PieceId piece_id, Side side, Position position) {
    Rook* rook = (Rook*)malloc(sizeof(Rook));

    // Set piece fields
    rook->piece.id = piece_id;
    rook->piece.side = side;
    rook->piece.type = PieceType_Rook;
    rook->piece.position = position;
    rook->piece.is_captured = 0;

    // Set piece functions
    rook->piece.Piece_Free = Rook_Free;

    // Set field
    rook->has_been_moved = 0;

    return rook;
}

Rook* Rook_Clone(Rook* rook_src) {
    Rook* rook = Rook_New(rook_src->piece.id, rook_src->piece.side, rook_src->piece.position);

    // Set piece fields
    rook->piece.is_captured = rook_src->piece.is_captured;

    // Set fields
    rook->has_been_moved = rook_src->has_been_moved;

    return rook;
}

void Rook_Free(Piece* piece) {
    Rook* rook = (Rook*)piece;
    free(rook);
}
