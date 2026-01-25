#include "pawn.h"

#include <stdlib.h>

#include "../board.h"
#include "../move/move.h"

const Vector2 PAWN_MOVE_UP_DIRECTIONS[] = {{-1, 0}, {-2, 0}};
const Vector2 PAWN_MOVE_TAKE_DIRECTIONS[] = {{-1, -1}, {-1, 1}};

void pawn_add_moves_up(Pawn*, Board*, MoveArray*);
void pawn_add_moves_take(Pawn*, Board*, MoveArray*);
void pawn_add_moves_en_passant(Pawn*, Board*, MoveArray*);
void pawn_add_move(Pawn*, MoveArray*, Move*);
void pawn_add_promotion_moves(MoveArray*, Move*);

Pawn* pawn_new(PieceId piece_id, Side side, Vector2 position) {
    Pawn* piece = (Pawn*)malloc(sizeof(Pawn));

    // Set piece fields
    piece->piece.id = piece_id;
    piece->piece.side = side;
    piece->piece.type = PIECE_TYPE_PAWN;
    piece->piece.position = position;
    piece->piece.is_captured = 0;

    // Set functions
    piece->piece.piece_get_positional_moves = pawn_get_positional_moves;
    piece->piece.piece_free = pawn_free;

    // Set fields
    piece->has_been_moved = 0;
    piece->first_time_moved = 0;

    return piece;
}

Pawn* pawn_clone(Pawn* pawn_src) {
    Pawn* pawn = pawn_new(pawn_src->piece.id, pawn_src->piece.side, pawn_src->piece.position);

    // Set piece fields
    pawn->piece.is_captured = pawn_src->piece.is_captured;

    // Set fields
    pawn->has_been_moved = pawn_src->has_been_moved;

    return pawn;
}

Pawn* pawn_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_PAWN) {
        return (Pawn*)piece;
    }
    return NULL;
}

MoveArray* pawn_get_positional_moves(Board* board, Piece* piece) {
    Pawn* pawn;
    MoveArray* move_array = move_array_new();

    if (!(pawn = pawn_cast(piece))) {
        return move_array;
    }

    pawn_add_moves_up(pawn, board, move_array);
    pawn_add_moves_take(pawn, board, move_array);
    pawn_add_moves_en_passant(pawn, board, move_array);

    return move_array;
}

void pawn_add_moves_up(Pawn* pawn, Board* board, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Vector2 position = pawn->piece.position;

    size_t total_directions = PAWN_MOVE_UP_TOTAL_DIRECTIONS;

    if (pawn->has_been_moved) {
        total_directions--;
    }

    for (size_t k = 0; k < total_directions; k++) {
        Vector2 direction = PAWN_MOVE_UP_DIRECTIONS[k];
        if (side == SIDE_BLACK) {
            direction = vector2_horizontal_flip(direction);
        }
        Vector2 position_to = vector2_add2(pawn->piece.position, direction);
        if (!is_position_in_boundary(position_to)) {
            break;
        }
        if (board_has_piece_on_position(board, position_to)) {
            break;
        }
        Move* move = move_new_moving_piece(pawn->piece.id, position_to);
        pawn_add_move(pawn, move_array, move);
    }
}

void pawn_add_moves_take(Pawn* pawn, Board* board, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Vector2 position = pawn->piece.position;

    for (size_t k = 0; k < PAWN_MOVE_TAKE_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = PAWN_MOVE_TAKE_DIRECTIONS[k];
        if (side == SIDE_BLACK) {
            direction = vector2_horizontal_flip(direction);
        }
        Vector2 position_to = vector2_add2(pawn->piece.position, direction);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!board_can_take_position(board, &pawn->piece, position_to)) {
            continue;
        }
        Piece* take_piece = board_get_piece_by_position(board, position_to);
        Move* move = move_new_taking_piece(pawn->piece.id, position_to, take_piece->id);
        pawn_add_move(pawn, move_array, move);
    }
}

void pawn_add_moves_en_passant(Pawn* pawn, Board* board, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Vector2 position = pawn->piece.position;

    for (size_t k = 0; k < PAWN_MOVE_TAKE_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = PAWN_MOVE_TAKE_DIRECTIONS[k];
        Vector2 direction_side_pawn = vector2_make(0, direction.j);
        if (side == SIDE_BLACK) {
            direction = vector2_horizontal_flip(direction);
        }
        Vector2 position_to = vector2_add2(pawn->piece.position, direction);
        Vector2 position_side_pawn = vector2_add2(pawn->piece.position, direction_side_pawn);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!is_position_in_boundary(position_side_pawn)) {
            continue;
        }
        Piece* piece = board_get_piece_by_position(board, position_side_pawn);
        Pawn* side_pawn;
        if (!(side_pawn = pawn_cast(piece))) {
            return;
        }
        if (!piece_is_opposite(&pawn->piece, piece)) {
            return;
        }
        if (!side_pawn->first_time_moved) {
            return;
        }
        Move* move = move_new_taking_piece(pawn->piece.id, position_to, side_pawn->piece.id);
        pawn_add_move(pawn, move_array, move);
    }
}

// Relaying to add single move or to add promotion moves
void pawn_add_move(Pawn* pawn, MoveArray* move_array, Move* move) {
    Side side = pawn->piece.side;

    uint8_t can_be_promoted = 0;
    if (side == SIDE_WHITE) {
        can_be_promoted = is_position_top(move->position_to);
    } else if (side == SIDE_BLACK) {
        can_be_promoted = is_position_bottom(move->position_to);
    }

    if (can_be_promoted) {
        pawn_add_promotion_moves(move_array, move);
    } else {
        move_array_add(move_array, move);
    }
}

// Clone move and configured with promotable piece, then add to array
void pawn_add_promotion_moves(MoveArray* move_array, Move* move) {
    Move* cloned_move = move_clone(move);
    move_with_promotion(cloned_move, PIECE_TYPE_QUEEN);
    move_array_add(move_array, cloned_move);

    cloned_move = move_clone(move);
    move_with_promotion(cloned_move, PIECE_TYPE_ROOK);
    move_array_add(move_array, cloned_move);

    cloned_move = move_clone(move);
    move_with_promotion(cloned_move, PIECE_TYPE_BISHOP);
    move_array_add(move_array, cloned_move);

    cloned_move = move_clone(move);
    move_with_promotion(cloned_move, PIECE_TYPE_KNIGHT);
    move_array_add(move_array, cloned_move);
}

void pawn_free(Piece* piece) {
    Pawn* pawn;
    if (!(pawn = pawn_cast(piece))) {
        return;
    }
    free(pawn);
}
