#include "knight.h"

#include <stdlib.h>

Knight* Knight_New(PieceId piece_id, Side side, Position position) {
    Knight* knight = (Knight*)malloc(sizeof(Knight));

    // Set piece fields
    knight->piece.id = piece_id;
    knight->piece.side = side;
    knight->piece.type = PieceType_Knight;
    knight->piece.position = position;
    knight->piece.is_captured = 0;

    // Set functions
    knight->piece.Piece_GetPositionalMoves = Knight_GetPositionalMoves;
    knight->piece.Piece_Free = Knight_Free;

    return knight;
}

Knight* Knight_Clone(Knight* knight_src) {
    Knight* knight = Knight_New(knight_src->piece.id, knight_src->piece.side, knight_src->piece.position);

    // Set piece fields
    knight->piece.is_captured = knight->piece.is_captured;

    return knight;
}

MoveArray* Knight_GetPositionalMoves(Board* board, Piece* piece) {
    Knight* knight = (Knight*)piece;
    // TODO: Find knight positional moves on board
}

void Knight_Free(Piece* piece) {
    Knight* knight = (Knight*)piece;
    free(knight);
}
