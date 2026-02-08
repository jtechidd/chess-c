#include "king.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "piece_macros.h"
#include "rook.h"

#define KING_TOTAL_DIRECTIONS 8
#define KING_MAX_SCALE 1
const vector2_t KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

piece_clone_fn king_piece_clone;
piece_get_moves_fn king_piece_get_moves;
piece_free_fn king_piece_free;

DEFINE_PIECE_NEW_INTERNAL(king, KING)
DEFINE_PIECE_CLONE_INTERNAL(king)
int _king_add_moves_positional(king_t *king, board_t *board, move_array_t *move_array) {
  int err;
  for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KING_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(king->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    bool has_piece_on_position;
    if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
      return err;
    }
    if (!has_piece_on_position) {
      move_array_add(move_array, move_new_moving_piece(king->piece.id, position_to));
      continue;
    }
    piece_t *piece;
    if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
      return err;
    }
    if (piece_is_opposite(&king->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(king->piece.id, position_to, piece->id));
    }
  }
  return CHESS_OK;
}
int _king_get_rook_to_castle_with(rook_t **rook_out, king_t *king, board_t *board, move_castling_type_t move_castling_type) {
  int err;
  piece_id_t rook_piece_id;
  if (king->piece.side == SIDE_WHITE) {
    if (move_castling_type == MOVE_CASTLING_TYPE_KING_SIDE) {
      rook_piece_id = PIECE_ID_WHITE_ROOK_2;
    } else if (move_castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
      rook_piece_id = PIECE_ID_WHITE_ROOK_1;
    }
  } else if (king->piece.side == SIDE_BLACK) {
    if (move_castling_type == MOVE_CASTLING_TYPE_KING_SIDE) {
      rook_piece_id = PIECE_ID_BLACK_ROOK_2;
    } else if (move_castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
      rook_piece_id = PIECE_ID_BLACK_ROOK_1;
    }
  }
  piece_t *piece_rook;
  if ((err = board_get_piece_by_id(&piece_rook, board, rook_piece_id)) != CHESS_OK) {
    return err;
  }
  rook_t *rook;
  if ((err = rook_piece_cast(&rook, piece_rook)) != CHESS_OK) {
    return err;
  }
  *rook_out = rook;
  return CHESS_OK;
}
int _king_add_moves_castle(king_t *king, board_t *board, move_array_t *move_array, move_castling_type_t move_castling_type) {
  int err;
  bool is_king_get_attacked;
  if ((err = board_is_king_get_attacked(&is_king_get_attacked, board, king->piece.side))) {
    return err;
  }
  if (is_king_get_attacked) {
    return CHESS_OK;
  }
  rook_t *rook;
  if ((err = _king_get_rook_to_castle_with(&rook, king, board, move_castling_type)) != CHESS_OK) {
    return err;
  }
  if (rook->piece.is_captured) {
    return CHESS_OK;
  }
  if (!(king->piece.moving_count == 0 && rook->piece.moving_count == 0)) {
    return CHESS_OK;
  }
  for (int j = 1; j <= 2; j++) {
    vector2_t check_direction = vector2_make(0, j);
    if (move_castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
      check_direction = vector2_hflip(check_direction);
    }
    vector2_t check_position = vector2_add2(king->piece.position, check_direction);
    bool is_position_safe_to_move_to;
    if ((err = board_is_position_safe_to_move_to(&is_position_safe_to_move_to, board, king->piece.side, check_position)) != CHESS_OK) {
      return err;
    }
    if (!is_position_safe_to_move_to) {
      return CHESS_OK;
    }
  }
  move_array_add(move_array, move_new_castling(king->piece.id, move_castling_type));
  return CHESS_OK;
}
int _king_get_moves(move_array_t **move_array_out, king_t *king, board_t *board) {
  if (!(move_array_out && king && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  if ((err = _king_add_moves_positional(king, board, move_array)) != CHESS_OK) {
    goto fail;
  }
  if ((err = _king_add_moves_castle(king, board, move_array, MOVE_CASTLING_TYPE_KING_SIDE)) != CHESS_OK) {
    goto fail;
  }
  if ((err = _king_add_moves_castle(king, board, move_array, MOVE_CASTLING_TYPE_QUEEN_SIDE)) != CHESS_OK) {
    goto fail;
  }

  *move_array_out = move_array;
  return CHESS_OK;
fail:
  if (move_array != NULL) {
    move_array_free(move_array);
    move_array = NULL;
  }
  return err;
}
DEFINE_PIECE_FREE_INTERNAL(king)

DEFINE_PIECE_NEW(king)
DEFINE_PIECE_CAST(king, KING)
DEFINE_PIECE_CLONE(king)
DEFINE_PIECE_GET_MOVES(king)
DEFINE_PIECE_FREE(king)

DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(king, KING)
