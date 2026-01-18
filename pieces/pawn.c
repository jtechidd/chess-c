#include "pawn.h"

#include <stdlib.h>

Pawn* Pawn_New(PieceId piece_id, Side side, Position position) {
    Pawn* pawn = (Pawn*)malloc(sizeof(Pawn));

    // Set piece fields
    pawn->piece.id = piece_id;
    pawn->piece.side = side;
    pawn->piece.type = PieceType_Pawn;
    pawn->piece.position = position;
    pawn->piece.is_captured = 0;

    // Set functions
    pawn->piece.Piece_Free = Pawn_Free;

    // Set fields
    pawn->has_been_moved = 0;

    return pawn;
}

Pawn* Pawn_Clone(Pawn* pawn_src) {
    Pawn* pawn = Pawn_New(pawn_src->piece.id, pawn_src->piece.side, pawn_src->piece.position);

    // Set piece fields
    pawn->piece.is_captured = pawn_src->piece.is_captured;

    // Set fields
    pawn->has_been_moved = pawn_src->has_been_moved;

    return pawn;
}

void Pawn_Free(Piece* p) {
    Pawn* pawn = (Pawn*)p;
    free(pawn);
}
