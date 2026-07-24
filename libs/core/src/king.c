#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"

static const ch_piece_methods_t CH_KING_METHODS;

void ch_chess_spawn_king(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_KING, position,
                       ch_piece_data_make_empty(), &CH_KING_METHODS);
}

static ch_error_t ch_king_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                        ch_move_t move,
                                        ch_piece_t **taking_piece) {
  return CH_ERR_SUCCESS;
}

static const ch_piece_methods_t CH_KING_METHODS = {
    .validate_move = ch_king_validate_move,
};