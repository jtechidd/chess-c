#include "king.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "rook.h"

#define KING_TOTAL_DIRECTIONS 8
const vector2_t KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

static piece_clone_fn _king_piece_clone;
static piece_get_moves_fn _king_piece_get_moves;
static piece_free_fn _king_piece_free;

static int _king_new(king_t **king_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(king_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }

  king_t *king = (king_t *)malloc(sizeof(king_t));
  if (!king) {
    return CHESS_ERROR_NO_MEMORY;
  }

  king->piece.id = piece_id;
  king->piece.side = side;
  king->piece.type = PIECE_TYPE_KING;
  king->piece.position = position;
  king->piece.is_captured = false;
  king->piece.moving_count = 0;

  king->piece.piece_clone = _king_piece_clone;
  king->piece.piece_free = _king_piece_free;
  king->piece.piece_get_moves = _king_piece_get_moves;

  *king_out = king;
  return CHESS_OK;
}

static int _king_clone(king_t **king_out, king_t *king_src) {
  if (!(king_out && king_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  king_t *king;
  if ((err = _king_new(&king, king_src->piece.id, king_src->piece.side, king_src->piece.position)) != CHESS_OK) {
    return err;
  }
  king->piece.is_captured = king_src->piece.is_captured;
  king->piece.moving_count = king_src->piece.moving_count;
  *king_out = king;
  return CHESS_OK;
}

static int _king_add_moves_positional(king_t *king, board_t *board, move_array_t *move_array) {
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

static int _king_get_rook_to_castle_with(rook_t **rook_out, king_t *king, board_t *board, move_castling_type_t move_castling_type) {
  if (!(rook_out && board && is_move_castling_type_valid(move_castling_type))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
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

static int _king_add_moves_castle(king_t *king, board_t *board, move_array_t *move_array, move_castling_type_t move_castling_type) {
  if (!(king && board && is_move_castling_type_valid(move_castling_type))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
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

static int _king_get_moves(move_array_t **move_array_out, king_t *king, board_t *board) {
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

static int _king_castle(king_t *king, move_t *move, board_t *board) {
  if (!(king && move && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  if (!(move->flags & MOVE_FLAGS_HAS_CASTLING)) {
    return CHESS_ERROR_INVALID_STATE;
  }
  rook_t *rook;
  if ((err = _king_get_rook_to_castle_with(&rook, king, board, move->castling_type)) != CHESS_OK) {
    return err;
  }
  if (rook->piece.is_captured) {
    return CHESS_ERROR_INVALID_STATE;
  }
  vector2_t king_position = king->piece.position;
  vector2_t king_next = vector2_make(0, 1);
  vector2_t king_next_2 = vector2_make(0, 2);
  if (move->castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
    king_next = vector2_hflip(king_next);
    king_next_2 = vector2_hflip(king_next_2);
  }
  king->piece.position = vector2_add2(king_position, king_next_2);
  rook->piece.position = vector2_add2(king_position, king_next);
  return CHESS_OK;
}

static int _king_free(king_t *king) {
  free(king);
  return CHESS_OK;
}

static int _king_piece_clone(piece_t **king_piece_out, piece_t *piece) {
  if (!(king_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  king_t *king, *cloned_king;
  if ((err = king_piece_cast(&king, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _king_clone(&cloned_king, king)) != CHESS_OK) {
    return err;
  }
  *king_piece_out = &cloned_king->piece;
  return CHESS_OK;
}

static int _king_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  king_t *king;
  if ((err = king_piece_cast(&king, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _king_get_moves(move_array_out, king, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _king_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  king_t *king;
  if ((err = king_piece_cast(&king, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _king_free(king)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int king_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  king_t *king;
  if ((err = _king_new(&king, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_t = &king->piece;
  return CHESS_OK;
}

int king_piece_cast(king_t **king_out, piece_t *piece) {
  if (!(king_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_KING) {
    *king_out = (king_t *)(piece - offsetof(king_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int king_piece_castle(piece_t *piece, move_t *move, board_t *board) {
  if (!(piece && move && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  king_t *king;
  if ((err = king_piece_cast(&king, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _king_castle(king, move, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int board_is_position_get_attacked_by_king(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KING_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(position, direction);
    if (!is_position_in_bound(position_to)) {
      break;
    }
    bool has_piece_on_position;
    if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
      return err;
    }
    if (!has_piece_on_position) {
      continue;
    }
    piece_t *piece;
    if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
      return err;
    }
    king_t *king;
    if ((err = king_piece_cast(&king, piece)) != CHESS_OK) {
      if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
        continue;
      } else {
        return err;
      }
    }
    if (is_opposite_side(side, king->piece.side)) {
      *bool_out = true;
      return CHESS_OK;
    }
  }
  *bool_out = false;
  return CHESS_OK;
}
