#include "piece.h"

static void piece_fill_moves_bishop(chess_t *chess, piece_t *piece) {}

const piece_methods_t PIECE_METHODS_BISHOP = {
    .piece_fill_moves = piece_fill_moves_bishop,
};
