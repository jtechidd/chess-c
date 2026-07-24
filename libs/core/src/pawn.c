#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static const ch_piece_methods_t CH_PAWN_METHODS;

void ch_chess_spawn_pawn(ch_chess_t *chess, ch_side_t side,
                         ch_vector2_t position) {
  ch_chess_spawn_piece(chess, side, CH_PIECE_TYPE_PAWN, position,
                       ch_piece_data_make_empty(), &CH_PAWN_METHODS);
}

static bool ch_can_do_en_passant(ch_chess_t *chess, ch_piece_t *pawn,
                                 ch_piece_t *en_passant_piece) {
  if (!en_passant_piece)
    return false;
  if (en_passant_piece->type != CH_PIECE_TYPE_PAWN)
    return false;
  if (en_passant_piece->side == pawn->side)
    return false;
  if (en_passant_piece->move_count != 1)
    return false;
  if (en_passant_piece->latest_move_turn_num != chess->num_turns - 1)
    return false;
  if (en_passant_piece->side == CH_SIDE_WHITE &&
      en_passant_piece->position.i != 4)
    return false;
  if (en_passant_piece->side == CH_SIDE_BLACK &&
      en_passant_piece->position.i != 3)
    return false;
  return true;
}

static bool ch_is_valid_promote_to(ch_piece_type_t type) {
  return type == CH_PIECE_TYPE_QUEEN || type == CH_PIECE_TYPE_ROOK ||
         type == CH_PIECE_TYPE_BISHOP || type == CH_PIECE_TYPE_KNIGHT;
}

static bool ch_is_position_promotable(ch_vector2_t pos, ch_side_t side) {
  return side == CH_SIDE_WHITE && pos.i == 0 ||
         side == CH_SIDE_BLACK && pos.i == CH_BOARD_HEIGHT - 1;
}

static ch_error_t ch_pawn_validate_move(ch_piece_t *pawn, ch_chess_t *chess,
                                        ch_move_t move,
                                        ch_validate_move_out_t *out) {
  ch_vector2_t up = ch_vector2_make(-1, 0);
  ch_vector2_t up2 = ch_vector2_make(-2, 0);
  ch_vector2_t left = ch_vector2_make(0, -1);
  ch_vector2_t right = ch_vector2_make(0, 1);
  ch_vector2_t up_left = ch_vector2_make(-1, -1);
  ch_vector2_t up_right = ch_vector2_make(-1, 1);

  if (pawn->side == CH_SIDE_BLACK) {
    up = ch_vector2_flipv(up);
    up2 = ch_vector2_flipv(up2);
    up_left = ch_vector2_flipv(up_left);
    up_right = ch_vector2_flipv(up_right);
  }

  ch_vector2_t disp = ch_vector2_sub(move.position_to, move.position_from);
  ch_vector2_t pos_from_up = ch_vector2_add(move.position_from, up);
  ch_vector2_t pos_from_up2 = ch_vector2_add(move.position_from, up2);
  ch_vector2_t pos_from_left = ch_vector2_add(move.position_from, left);
  ch_vector2_t pos_from_right = ch_vector2_add(move.position_from, right);

  ch_piece_t *en_passant_piece = NULL;

  if (move.is_taking) {
    if (out->piece_taking == NULL) {
      if (ch_vector2_equal(disp, up_left)) {
        en_passant_piece = ch_chess_get_piece_on_position(chess, pos_from_left);
      } else if (ch_vector2_equal(disp, up_right)) {
        en_passant_piece =
            ch_chess_get_piece_on_position(chess, pos_from_right);
      } else {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (!ch_can_do_en_passant(chess, pawn, en_passant_piece)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      out->piece_taking = en_passant_piece;
    } else {
      if (!ch_vector2_equal(disp, up_left) &&
          !ch_vector2_equal(disp, up_right)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    }
  } else {
    disp = ch_vector2_sub(move.position_to, move.position_from);
    if (ch_vector2_equal(disp, up2)) {
      if (pawn->move_count > 0) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (ch_chess_get_piece_on_position(chess, pos_from_up) ||
          ch_chess_get_piece_on_position(chess, pos_from_up2)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    } else if (ch_vector2_equal(disp, up)) {
      if (ch_chess_get_piece_on_position(chess, pos_from_up)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    } else {
      return CH_ERR_ILLEGAL_MOVE;
    }
  }

  if (ch_is_position_promotable(move.position_to, pawn->side)) {
    if (!ch_is_valid_promote_to(move.promote_to)) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  } else if (move.promote_to != CH_EMPTY) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  return CH_ERR_SUCCESS;
}

static const ch_piece_methods_t CH_PAWN_METHODS = {
    .validate_move = ch_pawn_validate_move,
};