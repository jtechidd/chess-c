#include "core/knight.h"
#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static const ch_piece_methods_t CH_KNIGHT_METHODS;

void ch_chess_spawn_knight(ch_chess_t *chess, ch_side_t side,
                           ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_KNIGHT, position,
                       ch_piece_data_make_empty(), &CH_KNIGHT_METHODS);
}

static bool ch_knight_check_displacement(ch_vector2_t disp) {
  if (disp.i != 0 && disp.j != 0 && abs(disp.i) + abs(disp.j) == 3)
    return true;
  return false;
}

static ch_error_t ch_knight_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                          ch_move_t move,
                                          ch_piece_t **taking_piece) {
  ch_vector2_t disp = ch_vector2_sub(move.position_to, move.position_from);
  if (!ch_knight_check_displacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  return CH_ERR_SUCCESS;
}

static const ch_piece_methods_t CH_KNIGHT_METHODS = {
    .validate_move = ch_knight_validate_move,
};