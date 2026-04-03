#include "piece.h"

static void piece_fill_moves_pawn(chess_t *chess, piece_t *piece) {}

const piece_methods_t PIECE_METHODS_PAWN = {
    .piece_fill_moves = piece_fill_moves_pawn,
};
