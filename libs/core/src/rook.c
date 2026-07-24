#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static const ch_piece_methods_t CH_ROOK_METHODS;

static ch_piece_data_t ch_piece_data_make_rook(ch_rook_type_t type) {
  return (ch_piece_data_t){.rook = {.type = type}};
}

void ch_chess_spawn_rook(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position, ch_rook_type_t type) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_ROOK, position,
                                  ch_piece_data_make_rook(type),
                                  &CH_ROOK_METHODS);
}

static bool CH_Rook_CheckDisplacement(ch_vector2_t disp) {
  return disp.i == 0 != disp.j == 0;
}

static ch_error_t CH_Rook_ValidateMove(ch_piece_t *piece, ch_chess_t *chess,
                                       ch_move_t move,
                                       ch_piece_t **takingPiece) {
  ch_vector2_t disp = ch_vector2_sub(move.position_to, move.position_from);
  if (!CH_Rook_CheckDisplacement(disp)) {
    return false;
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

static const ch_piece_methods_t CH_ROOK_METHODS = {
    .validate_move = CH_Rook_ValidateMove,
};
