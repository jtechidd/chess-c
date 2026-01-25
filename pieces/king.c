#include "king.h"

#include <stdlib.h>

#include "../board.h"
#include "../utils.h"

const Vector2 KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

King* king_new(PieceId piece_id, Side side, Vector2 position) {
    King* king = (King*)malloc(sizeof(King));

    // Set piece fields
    king->piece.id = piece_id;
    king->piece.side = side;
    king->piece.type = PIECE_TYPE_KING;
    king->piece.position = position;
    king->piece.is_captured = 0;

    // Set functions
    king->piece.piece_get_positional_moves = king_get_positional_moves;
    king->piece.piece_free = king_free;

    // Set fields
    king->has_been_moved = 0;

    return king;
}

King* king_clone(King* king_src) {
    King* king = king_new(king_src->piece.id, king_src->piece.side, king_src->piece.position);

    // Set piece fields
    king->piece.is_captured = king_src->piece.is_captured;

    // Set fields
    king->has_been_moved = king_src->has_been_moved;

    return king;
}

King* king_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_KING) {
        return (King*)piece;
    }
    return NULL;
}

MoveArray* king_get_positional_moves(Piece* piece, Board* board) {
    King* king;
    MoveArray* move_array = move_array_new();

    if (!(king = king_cast(piece))) {
        return move_array;
    }

    for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = KING_DIRECTIONS[k];
        Vector2 position_to = vector2_add2(king->piece.position, direction);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!board_has_piece_on_position(board, position_to)) {
            move_array_add(move_array, move_new_moving_piece(king->piece.id, position_to));
            continue;
        }
        Piece* piece = board_get_piece_by_position(board, position_to);
        if (piece_is_opposite(&king->piece, piece)) {
            move_array_add(move_array, move_new_taking_piece(king->piece.id, position_to, piece->id));
        }
    }
    return move_array;
}

void king_free(Piece* piece) {
    King* king;
    if (!(king = king_cast(piece))) {
        return;
    }
    free(king);
}

uint8_t board_is_position_get_attacked_by_king(Board* board, Side side, Vector2 position) {
    for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = KING_DIRECTIONS[k];
        Vector2 position_to = vector2_add2(position, direction);
        if (!is_position_in_boundary(position_to)) {
            continue;
        }
        if (!board_has_piece_on_position(board, position_to)) {
            continue;
        }
        Piece* piece = board_get_piece_by_position(board, position_to);
        King* king;
        if (!(king = king_cast(piece))) {
            continue;
        }
        if (is_opposite_side(side, king->piece.side)) {
            return 1;
        }
    }
    return 0;
}
