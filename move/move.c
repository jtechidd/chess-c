#include "move.h"

#include <stdio.h>
#include <stdlib.h>

Move* move_new() {
    Move* move = (Move*)malloc(sizeof(Move));
    move->flags = 0;
    return move;
}

Move* move_clone(Move* move) {
    Move* cloned_move = (Move*)malloc(sizeof(Move));

    cloned_move->flags = move->flags;

    cloned_move->piece_id = move->piece_id;
    cloned_move->position_to = move->position_to;

    cloned_move->take_piece_id = move->piece_id;

    cloned_move->promote_to = move->promote_to;

    cloned_move->castling_type = move->castling_type;
}

Move* move_new_moving_piece(PieceId piece_id, Vector2 position_to) {
    Move* move = move_new();
    move_with_moving_piece(move, piece_id, position_to);
    return move;
}

Move* move_new_taking_piece(PieceId piece_id, Vector2 position_to, PieceId take_piece_id) {
    Move* move = move_new_moving_piece(piece_id, position_to);
    move_with_taking_piece(move, take_piece_id);
}

Move* move_new_castling(MoveCastlingType castling_type) {
    Move* move = move_new();
    move->flags = MOVE_FLAGS_HAS_CASTLING;
    move->castling_type = castling_type;
    return move;
}

void move_with_moving_piece(Move* move, PieceId piece_id, Vector2 position_to) {
    move->flags |= MOVE_FLAGS_HAS_MOVING_PIECE;

    move->piece_id = piece_id;
    move->position_to = position_to;
}

void move_with_taking_piece(Move* move, PieceId take_piece_id) {
    move->flags |= MOVE_FLAGS_HAS_TAKING_PIECE;

    move->take_piece_id = take_piece_id;
}

void move_with_promotion(Move* move, PieceType promote_to) {
    move->flags |= MOVE_FLAGS_HAS_PROMOTION;

    move->promote_to = promote_to;
}

void move_debug(Move* move) {
    if (move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) {
        printf("piece_id=%d position=(%d,%d) ", move->piece_id, move->position_to.i, move->position_to.j);
    }
    if (move->flags & MOVE_FLAGS_HAS_TAKING_PIECE) {
        printf("take_piece_id=%d", move->take_piece_id);
    }
    if (move->flags & MOVE_FLAGS_HAS_PROMOTION) {
        printf("promote_to=%d ", move->promote_to);
    }
    if (move->flags & MOVE_FLAGS_HAS_CASTLING) {
        printf("castling_type=%d ", move->castling_type);
    }
    printf("\n");
}

void move_free(Move* move) {
    free(move);
}