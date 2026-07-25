#include "core/pieces/knight.h"
#include "core/chess.h"
#include "core/common.h"
#include "core/move_db.h"
#include "core/piece.h"
#include "core/utils.h"
#include "core/vector2.h"

#define CH_KNIGHT_NUM_DIRECTIONS 8

static const ch_piece_methods_t CH_KNIGHT_METHODS;
static const ch_vector2_t CH_KNIGHT_DIRECTIONS[] = {
    {-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, 1}, {2, -1}, {1, -2}, {-1, -2}};

void ch_chess_spawn_knight(ch_chess_t *chess, ch_side_t side,
                           ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_KNIGHT, position,
                       ch_piece_data_make_empty(), &CH_KNIGHT_METHODS);
}

static bool ch_knight_check_displacement(ch_vector2_t disp) {
  ch_vector2_t dir;
  for (uint8_t k = 0; k < CH_KNIGHT_NUM_DIRECTIONS; k++) {
    dir = CH_KNIGHT_DIRECTIONS[k];
    if (ch_vector2_equal(disp, dir)) {
      return true;
    }
  }
  return false;
}

static ch_error_t ch_knight_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                          ch_move_t move,
                                          ch_apply_move_payload_t *payload) {
  ch_vector2_t disp = ch_vector2_sub(move.position_to, move.position_from);
  if (!ch_knight_check_displacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  return CH_ERR_SUCCESS;
}

bool ch_chess_is_position_safe_from_knight(ch_chess_t *chess,
                                           ch_vector2_t position) {
  ch_vector2_t dest, dir;
  ch_piece_t *piece;
  for (uint8_t k = 0; k < CH_KNIGHT_NUM_DIRECTIONS; k++) {
    dir = CH_KNIGHT_DIRECTIONS[k];
    dest = ch_vector2_add(position, dir);
    if (!ch_is_position_in_bound(dest)) {
      continue;
    }
    piece = ch_chess_get_piece_on_position(chess, dest);
    if (piece == NULL) {
      continue;
    }
    if (piece->type == CH_PIECE_TYPE_KNIGHT && piece->side != chess->turn) {
      return false;
    }
  }
  return true;
}

static void ch_knight_fill_moves(ch_piece_t *knight, ch_chess_t *chess,
                                 ch_move_db_t *move_db) {
  ch_vector2_t dest, dir;
  ch_move_t move;
  for (uint8_t k = 0; k < CH_KNIGHT_NUM_DIRECTIONS; k++) {
    dir = CH_KNIGHT_DIRECTIONS[k];
    dest = ch_vector2_add(knight->position, dir);
    if (!ch_is_position_in_bound(dest)) {
      continue;
    }
    move = ch_move_make_from_piece(knight, false, dest, CH_EMPTY);
    ch_chess_validate_and_add_move(chess, move_db, move);

    move = ch_move_make_from_piece(knight, true, dest, CH_EMPTY);
    ch_chess_validate_and_add_move(chess, move_db, move);
  }
  return;
}

static const ch_piece_methods_t CH_KNIGHT_METHODS = {
    .validate_move = ch_knight_validate_move,
    .fill_moves = ch_knight_fill_moves,
};