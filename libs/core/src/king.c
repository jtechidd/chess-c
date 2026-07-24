#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/utils.h"
#include "core/vector2.h"
#include <assert.h>

static const ch_piece_methods_t CH_KING_METHODS;

void ch_chess_spawn_king(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_KING, position,
                       ch_piece_data_make_empty(), &CH_KING_METHODS);
}

static bool ch_king_check_displacement_castling(ch_vector2_t disp) {
  return disp.i == 0 && abs(disp.j) == 2;
}

static bool ch_king_check_displacement(ch_vector2_t disp) {
  if (abs(disp.i) + abs(disp.j) == 1 || abs(disp.i) * abs(disp.j) == 1)
    return true;
  return false;
}

static ch_error_t ch_king_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                        ch_move_t move,
                                        ch_validate_move_out_t *out) {
  ch_vector2_t disp, dir, adj, dest;
  disp = ch_vector2_sub(move.position_to, move.position_from);
  if (ch_king_check_displacement_castling(disp)) {
    if (move.is_taking) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    if (piece->move_count > 0) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    dir = ch_vector2_make(0, disp.j / abs(disp.j));
    adj = ch_vector2_add(move.position_from, dir);
    if (!(ch_chess_is_position_safe(chess, move.position_to) &&
          ch_chess_is_position_safe(chess, adj))) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    for (uint8_t k = 3; k < 8; k++) {
      dest = ch_vector2_add(move.position_from, ch_vector2_scalmult(dir, k));
      if (!ch_is_position_in_bound(dest)) {
        break;
      }
      piece = ch_chess_get_piece_on_position(chess, dest);
      if (piece == NULL) {
        continue;
      }
      if (piece->type != CH_PIECE_TYPE_ROOK) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (piece->move_count > 0) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      out->piece_castling_rook = piece;
      out->piece_castling_rook_position_to = adj;
      return CH_ERR_SUCCESS;
    }
    return CH_ERR_ILLEGAL_MOVE;
  } else if (!ch_king_check_displacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  return CH_ERR_SUCCESS;
}

static const ch_piece_methods_t CH_KING_METHODS = {
    .validate_move = ch_king_validate_move,
};