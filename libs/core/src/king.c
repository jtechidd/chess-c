#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/utils.h"
#include "core/vector2.h"

#define CH_KING_NUM_DIRECTIONS 8
#define CH_KING_NUM_CASTLING_DIRECTIONS 2

static const ch_piece_methods_t CH_KING_METHODS;
static const ch_vector2_t CH_KING_DIRECTIONS[] = {
    {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};
static const ch_vector2_t CH_KING_CASTLING_DIRECTIONS[] = {{0, 1}, {0, -1}};

void ch_chess_spawn_king(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_KING, position,
                       ch_piece_data_make_empty(), &CH_KING_METHODS);
}

static bool ch_king_check_displacement_castling(ch_vector2_t disp,
                                                ch_vector2_t *dir_out) {
  ch_vector2_t dir;
  for (uint8_t k = 0; k < CH_KING_NUM_CASTLING_DIRECTIONS; k++) {
    dir = CH_KING_CASTLING_DIRECTIONS[k];
    if (ch_vector2_equal(disp, ch_vector2_scalmult(dir, 2))) {
      *dir_out = dir;
      return true;
    }
  }
  return false;
}

static bool ch_king_check_displacement(ch_vector2_t disp) {
  ch_vector2_t dir;
  for (uint8_t k = 0; k < CH_KING_NUM_DIRECTIONS; k++) {
    dir = CH_KING_DIRECTIONS[k];
    if (ch_vector2_equal(disp, dir)) {
      return true;
    }
  }
  return false;
}

static ch_error_t ch_king_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                        ch_move_t move,
                                        ch_apply_move_payload_t *payload) {
  ch_vector2_t disp, dir, adj, dest;
  ch_piece_t *castling_rook;
  disp = ch_vector2_sub(move.position_to, move.position_from);
  if (ch_king_check_displacement_castling(disp, &dir)) {
    if (move.is_taking) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    if (piece->move_count > 0) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    adj = ch_vector2_add(move.position_from, dir);
    if (!(ch_chess_is_position_safe_to_move_to(chess, move.position_to) &&
          ch_chess_is_position_safe_to_move_to(chess, adj))) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    for (uint8_t k = 3; k < CH_BOARD_SIZE; k++) {
      dest = ch_vector2_add(move.position_from, ch_vector2_scalmult(dir, k));
      if (!ch_is_position_in_bound(dest)) {
        break;
      }
      castling_rook = ch_chess_get_piece_on_position(chess, dest);
      if (castling_rook == NULL) {
        continue;
      }
      if (castling_rook->type != CH_PIECE_TYPE_ROOK) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (castling_rook->move_count > 0) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      payload->castling_rook_id = castling_rook->id;
      payload->castling_rook_position_to = adj;
      return CH_ERR_SUCCESS;
    }
    return CH_ERR_ILLEGAL_MOVE;
  } else if (!ch_king_check_displacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  return CH_ERR_SUCCESS;
}

bool ch_chess_is_position_safe_from_king(ch_chess_t *chess,
                                         ch_vector2_t position) {
  ch_vector2_t dest, dir;
  ch_piece_t *piece;
  for (uint8_t k = 0; k < CH_KING_NUM_DIRECTIONS; k++) {
    dir = CH_KING_DIRECTIONS[k];
    dest = ch_vector2_add(position, dir);
    if (!ch_is_position_in_bound(dest)) {
      continue;
    }
    piece = ch_chess_get_piece_on_position(chess, dest);
    if (piece == NULL) {
      continue;
    }
    if (piece->type == CH_PIECE_TYPE_KING && piece->side != chess->turn) {
      return false;
    }
  }
  return true;
}

static void ch_king_fill_moves(ch_piece_t *king, ch_chess_t *chess,
                               ch_move_db_t *move_db) {
  ch_vector2_t dest, dir;
  ch_move_t move;
  for (uint8_t k = 0; k < CH_KING_NUM_DIRECTIONS; k++) {
    dir = CH_KING_DIRECTIONS[k];
    dest = ch_vector2_add(king->position, dir);
    if (!ch_is_position_in_bound(dest)) {
      continue;
    }
    move = ch_move_make_from_piece(king, false, dest, CH_EMPTY);
    ch_chess_validate_and_add_move(chess, move_db, move);

    move = ch_move_make_from_piece(king, true, dest, CH_EMPTY);
    ch_chess_validate_and_add_move(chess, move_db, move);
  }

  for (uint8_t k = 0; k < CH_KING_NUM_CASTLING_DIRECTIONS; k++) {
    dir = CH_KING_CASTLING_DIRECTIONS[k];
    dest = ch_vector2_add(king->position, ch_vector2_scalmult(dir, 2));
    if (!ch_is_position_in_bound(dest)) {
      continue;
    }

    move = ch_move_make_from_piece(king, false, dest, CH_EMPTY);
    ch_chess_validate_and_add_move(chess, move_db, move);
  }
  return;
}

static const ch_piece_methods_t CH_KING_METHODS = {
    .validate_move = ch_king_validate_move,
    .fill_moves = ch_king_fill_moves,
};