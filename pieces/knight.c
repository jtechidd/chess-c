#include "knight.h"

#include <stdlib.h>

#include "../board.h"
#include "../utils.h"

const Vector2 KNIGHT_DIRECTIONS[] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}};

Knight* knight_new(PieceId piece_id, Side side, Vector2 position) {
    Knight* knight = (Knight*)malloc(sizeof(Knight));

    // Set piece fields
    knight->piece.id = piece_id;
    knight->piece.side = side;
    knight->piece.type = PIECE_TYPE_KNIGHT;
    knight->piece.position = position;
    knight->piece.is_captured = 0;

    // Set functions
    knight->piece.piece_get_positional_moves = knight_get_positional_moves;
    knight->piece.piece_free = knight_free;

    return knight;
}

Knight* knight_clone(Knight* knight_src) {
    Knight* knight = knight_new(knight_src->piece.id, knight_src->piece.side, knight_src->piece.position);

    // Set piece fields
    knight->piece.is_captured = knight->piece.is_captured;

    return knight;
}

Knight* knight_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_KNIGHT) {
        return (Knight*)piece;
    }
    return NULL;
}

MoveArray* knight_get_positional_moves(Piece* piece, Board* board) {
    Knight* knight;
    MoveArray* move_array = move_array_new();

    if (!(knight = knight_cast(piece))) {
        return move_array;
    }

    for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = KNIGHT_DIRECTIONS[k];
        Vector2 position_to = vector2_add2(knight->piece.position, direction);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!board_has_piece_on_position(board, position_to)) {
            move_array_add(move_array, move_new_moving_piece(knight->piece.id, position_to));
            continue;
        }
        Piece* piece = board_get_piece_by_position(board, position_to);
        if (piece_is_opposite(&knight->piece, piece)) {
            move_array_add(move_array, move_new_taking_piece(knight->piece.id, position_to, piece->id));
        }
    }
    return move_array;
}

void knight_free(Piece* piece) {
    Knight* knight;
    if (!(knight = knight_cast(piece))) {
        return;
    }
    free(knight);
}

uint8_t board_is_position_get_attacked_by_knight(Board* board, Side side, Vector2 position) {
    for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = KNIGHT_DIRECTIONS[k];
        Vector2 position_to = vector2_add2(position, direction);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!board_has_piece_on_position(board, position_to)) {
            continue;
        }
        Piece* piece = board_get_piece_by_position(board, position_to);
        Knight* knight;
        if (!(knight = knight_cast(piece))) {
            continue;
        }
        if (is_opposite_side(side, knight->piece.side)) {
            return 1;
        }
    }
    return 0;
}