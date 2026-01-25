#include "bishop.h"

#include <stdlib.h>

#include "../board.h"

const Vector2 BISHOP_DIRECTIONS[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

Bishop* bishop_new(PieceId pieceid, Side side, Vector2 position) {
    Bishop* bishop = (Bishop*)malloc(sizeof(Bishop));

    // Set piece fields
    bishop->piece.id = pieceid;
    bishop->piece.side = side;
    bishop->piece.type = PIECE_TYPE_BISHOP;
    bishop->piece.position = position;
    bishop->piece.is_captured = 0;

    // Set functions
    bishop->piece.piece_free = bishop_free;
    bishop->piece.piece_get_positional_moves = bishop_get_positional_moves;

    return bishop;
}

Bishop* bishop_clone(Bishop* bishop_src) {
    Bishop* bishop = bishop_new(bishop_src->piece.id, bishop_src->piece.side, bishop_src->piece.position);

    // Set piece fields
    bishop->piece.is_captured = bishop_src->piece.is_captured;

    return bishop;
}

Bishop* bishop_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_BISHOP) {
        return (Bishop*)piece;
    }
    return NULL;
}

MoveArray* bishop_get_positional_moves(Board* board, Piece* piece) {
    Bishop* bishop;
    MoveArray* move_array = move_array_new();

    if (!(bishop = bishop_cast(piece))) {
        return move_array;
    }

    for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = BISHOP_DIRECTIONS[k];
        for (uint8_t scale = 1;; scale++) {
            Vector2 position_to = vector2_add2(bishop->piece.position, vector2_scalar_multiply(direction, scale));
            if (!is_position_in_boundary(position_to)) {
                break;
            }
            if (!board_has_piece_on_position(board, position_to)) {
                move_array_add(move_array, move_new_moving_piece(bishop->piece.id, position_to));
                continue;
            }
            Piece* piece = board_get_piece_by_position(board, position_to);
            if (piece_is_opposite(&bishop->piece, piece)) {
                move_array_add(move_array, move_new_taking_piece(bishop->piece.id, position_to, piece->id));
            }
            break;
        }
    }

    return move_array;
}

void bishop_free(Piece* piece) {
    Bishop* bishop;
    if (!(bishop = bishop_cast(piece))) {
        return;
    }
    free(bishop);
}
