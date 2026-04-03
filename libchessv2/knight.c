#include "piece.h"

static void piece_fill_moves_knight(chess_t *chess, piece_t *piece) {}

const piece_methods_t PIECE_METHODS_KNIGHT = {
    .piece_fill_moves = piece_fill_moves_knight,
};
