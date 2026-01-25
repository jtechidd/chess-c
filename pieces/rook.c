#include "rook.h"

#include "../board.h"
#include "../utils.h"

const Vector2 ROOK_DIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

Rook* rook_new(PieceId piece_id, Side side, Vector2 position) {
    Rook* rook = (Rook*)malloc(sizeof(Rook));

    // Set piece fields
    rook->piece.id = piece_id;
    rook->piece.side = side;
    rook->piece.type = PIECE_TYPE_ROOK;
    rook->piece.position = position;
    rook->piece.is_captured = 0;

    // Set piece functions
    rook->piece.piece_get_positional_moves = rook_get_positional_moves;
    rook->piece.piece_free = rook_free;

    // Set field
    rook->has_been_moved = 0;

    return rook;
}

Rook* rook_clone(Rook* rook_src) {
    Rook* rook = rook_new(rook_src->piece.id, rook_src->piece.side, rook_src->piece.position);

    // Set piece fields
    rook->piece.is_captured = rook_src->piece.is_captured;

    // Set fields
    rook->has_been_moved = rook_src->has_been_moved;

    return rook;
}

Rook* rook_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_ROOK) {
        return (Rook*)piece;
    }
    return NULL;
}

MoveArray* rook_get_positional_moves(Piece* piece, Board* board) {
    Rook* rook;
    MoveArray* move_array = move_array_new();

    if (!(rook = rook_cast(piece))) {
        return move_array;
    }

    for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = ROOK_DIRECTIONS[k];
        for (uint8_t scale = 1;; scale++) {
            Vector2 position_to = vector2_add2(rook->piece.position, vector2_scalar_multiply(direction, scale));
            if (!is_position_in_boundary(position_to)) {
                break;
            }
            if (!board_has_piece_on_position(board, position_to)) {
                move_array_add(move_array, move_new_moving_piece(rook->piece.id, position_to));
                continue;
            }
            Piece* piece = board_get_piece_by_position(board, position_to);
            if (piece_is_opposite(&rook->piece, piece)) {
                move_array_add(move_array, move_new_taking_piece(rook->piece.id, position_to, piece->id));
            }
            break;
        }
    }

    return move_array;
}

void rook_free(Piece* piece) {
    Rook* rook;
    if (!(rook = rook_cast(piece))) {
        return;
    }
    free(rook);
}

uint8_t board_is_position_get_attacked_by_rook(Board* board, Side side, Vector2 position) {
    for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = ROOK_DIRECTIONS[k];
        for (uint8_t scale = 1;; scale++) {
            Vector2 position_to = vector2_add2(position, vector2_scalar_multiply(direction, scale));
            if (!is_position_in_boundary(position_to)) {
                break;
            }
            if (!board_has_piece_on_position(board, position_to)) {
                continue;
            }
            Piece* piece = board_get_piece_by_position(board, position_to);
            Rook* rook;
            if (!(rook = rook_cast(piece))) {
                break;
            }
            if (is_opposite_side(side, rook->piece.side)) {
                return 1;
            }
        }
    }
    return 0;
}