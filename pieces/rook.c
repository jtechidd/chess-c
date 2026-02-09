#include "rook.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define ROOK_TOTAL_DIRECTIONS 4
const vector2_t ROOK_DIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

static piece_clone_fn _rook_piece_clone;
static piece_get_moves_fn _rook_piece_get_moves;
static piece_free_fn _rook_piece_free;

static int _rook_new(rook_t **rook_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(rook_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  rook_t *rook = (rook_t *)malloc(sizeof(rook_t));
  if (!rook) {
    return CHESS_ERROR_NO_MEMORY;
  }
  rook->piece.id = piece_id;
  rook->piece.side = side;
  rook->piece.type = PIECE_TYPE_ROOK;
  rook->piece.position = position;
  rook->piece.is_captured = false;
  rook->piece.moving_count = 0;

  rook->piece.piece_clone = _rook_piece_clone;
  rook->piece.piece_free = _rook_piece_free;
  rook->piece.piece_get_moves = _rook_piece_get_moves;

  *rook_out = rook;
  return CHESS_OK;
}

static int _rook_clone(rook_t **rook_out, rook_t *rook_src) {
  if (!(rook_out && rook_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  rook_t *rook;
  if ((err = _rook_new(&rook, rook_src->piece.id, rook_src->piece.side, rook_src->piece.position)) != CHESS_OK) {
    return err;
  }
  rook->piece.is_captured = rook_src->piece.is_captured;
  rook->piece.moving_count = rook_src->piece.moving_count;
  *rook_out = rook;
  return CHESS_OK;
}

static int _rook_get_moves(move_array_t **move_array_out, rook_t *rook, board_t *board) {
  if (!(move_array_out && rook && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(rook->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      bool has_piece_on_position;
      if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (!has_piece_on_position) {
        move_array_add(move_array, move_new_moving_piece(rook->piece.id, position_to));
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (piece_is_opposite(&rook->piece, piece)) {
        move_array_add(move_array, move_new_taking_piece(rook->piece.id, position_to, piece->id));
      }
      break;
    }
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

static int _rook_free(rook_t *rook) {
  free(rook);
  return CHESS_OK;
}

static int _rook_piece_clone(piece_t **rook_piece_out, piece_t *piece) {
  if (!(rook_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  rook_t *rook, *cloned_rook;
  if ((err = rook_piece_cast(&rook, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _rook_clone(&cloned_rook, rook)) != CHESS_OK) {
    return err;
  }
  *rook_piece_out = &cloned_rook->piece;
  return CHESS_OK;
}

static int _rook_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  rook_t *rook;
  if ((err = rook_piece_cast(&rook, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _rook_get_moves(move_array_out, rook, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _rook_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  rook_t *rook;
  if ((err = rook_piece_cast(&rook, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _rook_free(rook)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int rook_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  rook_t *rook;
  if ((err = _rook_new(&rook, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_t = &rook->piece;
  return CHESS_OK;
}

int rook_piece_cast(rook_t **rook_out, piece_t *piece) {
  if (!(rook_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_ROOK) {
    *rook_out = (rook_t *)(piece - offsetof(rook_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int board_is_position_get_attacked_by_rook(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(position, vector2_scaled(direction, scale));
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
      rook_t *rook;
      if ((err = rook_piece_cast(&rook, piece)) != CHESS_OK) {
        if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
          break;
        } else {
          return err;
        }
      }
      if (is_opposite_side(side, rook->piece.side)) {
        *bool_out = true;
        return 0;
      }
    }
  }
  *bool_out = false;
  return 0;
}
