#include "core/pieces/rook.h"
#include "core/chess.h"
#include "core/common.h"
#include "core/move.h"
#include "core/move_db.h"
#include "core/piece.h"
#include "core/utils.h"
#include "core/vector2.h"
#include <stdbool.h>

#define CH_ROOK_NUM_DIRECTIONS 4

static const ch_piece_methods_t CH_ROOK_METHODS;
static const ch_vector2_t CH_ROOK_DIRECTIONS[] = {
    {-1, 0}, {0, 1}, {1, 0}, {0, -1}};

void ch_chess_spawn_rook(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_ROOK, position,
                       ch_piece_data_make_empty(), &CH_ROOK_METHODS);
}

static bool ch_rook_check_displacement(ch_vector2_t disp,
                                       ch_vector2_t *dir_out) {
  ch_vector2_t dir;
  for (uint8_t k = 0; k < CH_ROOK_NUM_DIRECTIONS; k++) {
    dir = CH_ROOK_DIRECTIONS[k];
    if (ch_vector2_parallel(disp, dir)) {
      *dir_out = dir;
      return true;
    }
  }
  return false;
}

static ch_error_t ch_rook_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                        ch_move_t move,
                                        ch_apply_move_payload_t *payload) {
  ch_vector2_t disp, dir, dest;
  disp = ch_vector2_sub(move.position_to, move.position_from);
  if (!ch_rook_check_displacement(disp, &dir)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  for (uint8_t k = 1; k < CH_BOARD_SIZE; k++) {
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

bool ch_chess_is_position_safe_from_rook(ch_chess_t *chess,
                                         ch_vector2_t position) {
  ch_vector2_t dir, dest;
  ch_piece_t *piece;
  for (uint8_t k = 0; k < CH_ROOK_NUM_DIRECTIONS; k++) {
    dir = CH_ROOK_DIRECTIONS[k];
    for (uint8_t s = 1; s < CH_BOARD_SIZE; s++) {
      dest = ch_vector2_add(position, ch_vector2_scalmult(dir, s));
      if (!ch_is_position_in_bound(dest)) {
        break;
      }
      piece = ch_chess_get_piece_on_position(chess, dest);
      if (piece == NULL) {
        continue;
      }
      if (piece->type == CH_PIECE_TYPE_ROOK && piece->side != chess->turn) {
        return false;
      }
      break;
    }
  }
  return true;
}

void ch_rook_fill_moves(ch_piece_t *rook, ch_chess_t *chess,
                            ch_move_db_t *move_db) {
  ch_vector2_t dir, dest;
  ch_move_t move;
  for (uint8_t k = 0; k < CH_ROOK_NUM_DIRECTIONS; k++) {
    dir = CH_ROOK_DIRECTIONS[k];
    for (uint8_t s = 1; s < CH_BOARD_SIZE; s++) {
      dest = ch_vector2_add(rook->position, ch_vector2_scalmult(dir, s));
      if (!ch_is_position_in_bound(dest)) {
        break;
      }

      move = ch_move_make_from_piece(rook, false, dest, CH_EMPTY);
      ch_chess_validate_and_add_move(chess, move_db, move);

      move = ch_move_make_from_piece(rook, true, dest, CH_EMPTY);
      ch_chess_validate_and_add_move(chess, move_db, move);
    }
  }
  return;
}

static const ch_piece_methods_t CH_ROOK_METHODS = {
    .validate_move = ch_rook_validate_move,
    .fill_moves = ch_rook_fill_moves,
};
