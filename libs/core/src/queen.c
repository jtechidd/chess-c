#include "core/queen.h"

#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"

static const ch_piece_methods_t CH_QUEEN_METHODS;

void ch_chess_spawn_queen(ch_chess_t *chess, ch_side_t side,
                          ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_QUEEN, position,
                       ch_piece_data_make_empty(), &CH_QUEEN_METHODS);
}

static bool ch_queen_check_displacement(ch_vector2_t disp) {
  if (disp.i == 0 != disp.j == 0)
    return true;
  if (disp.i != 0 && disp.j != 0 && abs(disp.i) == abs(disp.j))
    return true;
  return false;
}

static ch_error_t ch_queen_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                         ch_move_t move,
                                         ch_piece_t **taking_piece) {
  ch_vector2_t disp = ch_vector2_sub(move.position_to, move.position_from);
  if (!ch_queen_check_displacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  ch_vector2_t dir = ch_vector2_make(disp.i == 0 ? 0 : (disp.i / abs(disp.i)),
                                     disp.j == 0 ? 0 : (disp.j / abs(disp.j)));
  ch_vector2_t dest;
  for (uint8_t k = 1; k < 8; k++) {
    dest = ch_vector2_add(move.position_from, ch_vector2_scalmult(dir, k));
    if (ch_vector2_equal(dest, move.position_to)) {
      break;
    }
    if (ch_chess_get_piece_on_position(chess, dest)) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  }

  return CH_ERR_SUCCESS;
}

static const ch_piece_methods_t CH_QUEEN_METHODS = {
    .validate_move = ch_queen_validate_move,
};