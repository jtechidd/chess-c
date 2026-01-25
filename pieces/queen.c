#include "queen.h"

#include <stdlib.h>

#include "../board.h"
#include "../utils.h"

const Vector2 QUEEN_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

Queen* queen_new(PieceId piece_id, Side side, Vector2 position) {
    Queen* queen = (Queen*)malloc(sizeof(Queen));

    // Set piece fields
    queen->piece.id = piece_id;
    queen->piece.side = side;
    queen->piece.type = PIECE_TYPE_QUEEN;
    queen->piece.position = position;
    queen->piece.is_captured = 0;

    // Set functions
    queen->piece.piece_get_positional_moves = queen_get_positional_moves;
    queen->piece.piece_free = queen_free;

    return queen;
}

Queen* queen_clone(Queen* queen_src) {
    Queen* queen = queen_new(queen_src->piece.id, queen_src->piece.side, queen_src->piece.position);

    // Set piece fields
    queen->piece.is_captured = queen_src->piece.is_captured;

    return queen;
}

Queen* queen_cast(Piece* piece) {
    if (piece && piece->type == PIECE_TYPE_QUEEN) {
        return (Queen*)piece;
    }
    return NULL;
}

MoveArray* queen_get_positional_moves(Piece* piece, Board* board) {
    Queen* queen;
    MoveArray* move_array = move_array_new();

    if (!(queen = queen_cast(piece))) {
        return move_array;
    }

    for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = QUEEN_DIRECTIONS[k];
        for (uint8_t scale = 1;; scale++) {
            Vector2 position_to = vector2_add2(queen->piece.position, vector2_scalar_multiply(direction, scale));
            if (!is_position_in_boundary(position_to)) {
                break;
            }
            if (!board_has_piece_on_position(board, position_to)) {
                move_array_add(move_array, move_new_moving_piece(queen->piece.id, position_to));
                continue;
            }
            Piece* piece = board_get_piece_by_position(board, position_to);
            if (piece_is_opposite(&queen->piece, piece)) {
                move_array_add(move_array, move_new_taking_piece(queen->piece.id, position_to, piece->id));
            }
            break;
        }
    }
    return move_array;
}

void queen_free(Piece* piece) {
    Queen* queen;
    if (!(queen = queen_cast(piece))) {
        return;
    }
    free(queen);
}

uint8_t board_is_position_get_attacked_by_queen(Board* board, Side side, Vector2 position) {
    for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
        Vector2 direction = QUEEN_DIRECTIONS[k];
        for (uint8_t scale = 1;; scale++) {
            Vector2 position_to = vector2_add2(position, vector2_scalar_multiply(direction, scale));
            if (!is_position_in_boundary(position_to)) {
                break;
            }
            if (!board_has_piece_on_position(board, position_to)) {
                continue;
            }
            Piece* piece = board_get_piece_by_position(board, position_to);
            Queen* queen;
            if (!(queen = queen_cast(piece))) {
                break;
            }
            if (is_opposite_side(side, queen->piece.side)) {
                return 1;
            }
        }
    }
    return 0;
}