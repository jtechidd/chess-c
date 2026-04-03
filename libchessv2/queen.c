#include "piece.h"

static void piece_fill_moves_queen(chess_t *chess, piece_t *piece) {}

const piece_methods_t PIECE_METHODS_QUEEN = {
    .piece_fill_moves = piece_fill_moves_queen,
};
