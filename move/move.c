#include "move.h"

#include <stdlib.h>

Move* Move_New() {
    Move* move = (Move*)malloc(sizeof(Move));
    move->flags = 0;
    return move;
}

Move* Move_Clone(Move* move) {
    Move* cloned_move = (Move*)malloc(sizeof(Move));

    cloned_move->flags = move->flags;

    cloned_move->piece_id = move->piece_id;
    cloned_move->position = move->position;

    cloned_move->take_piece_id = move->piece_id;

    cloned_move->promote_to = move->promote_to;

    cloned_move->castling_type = move->castling_type;
}

Move* Move_NewMovingPiece(PieceId piece_id, Position position) {
    Move* move = Move_New();
    Move_WithMovingPiece(move, piece_id, position);
    return move;
}

Move* Move_NewTakingPiece(PieceId piece_id, Position position, PieceId take_piece_id) {
    Move* move = Move_NewMovingPiece(piece_id, position);
    Move_WithTakingPiece(move, take_piece_id);
}

Move* Move_NewCastling(MoveCastlingType castling_type) {
    Move* move = Move_New();
    move->flags = MoveFlags_HasCastling;
    move->castling_type = castling_type;
    return move;
}

void Move_WithMovingPiece(Move* move, PieceId piece_id, Position position) {
    move->flags |= MoveFlags_HasMovingPiece;

    move->piece_id = piece_id;
    move->position = position;
}

void Move_WithTakingPiece(Move* move, PieceId take_piece_id) {
    move->flags |= MoveFlags_HasTakingPiece;

    move->take_piece_id = take_piece_id;
}

void Move_WithPromotion(Move* move, PieceType promote_to) {
    move->flags |= MoveFlags_HasPromotion;

    move->promote_to = promote_to;
}

void Move_Free(Move* move) {
    free(move);
}