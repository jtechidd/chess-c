#include "bishop.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define BISHOP_TOTAL_DIRECTIONS 4
const vector2_t BISHOP_DIRECTIONS[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

static piece_clone_fn _bishop_piece_clone;
static piece_get_moves_fn _bishop_piece_get_moves;
static piece_free_fn _bishop_piece_free;

static int _bishop_new(bishop_t **bishop_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(bishop_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }

  bishop_t *bishop = (bishop_t *)malloc(sizeof(bishop_t));
  if (!bishop) {
    return CHESS_ERROR_NO_MEMORY;
  }

  bishop->piece.id = piece_id;
  bishop->piece.side = side;
  bishop->piece.type = PIECE_TYPE_BISHOP;
  bishop->piece.position = position;
  bishop->piece.is_captured = false;
  bishop->piece.moving_count = 0;

  bishop->piece.piece_clone = _bishop_piece_clone;
  bishop->piece.piece_free = _bishop_piece_free;
  bishop->piece.piece_get_moves = _bishop_piece_get_moves;

  *bishop_out = bishop;
  return CHESS_OK;
}

static int _bishop_clone(bishop_t **bishop_out, bishop_t *bishop_src) {
  if (!(bishop_out && bishop_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  bishop_t *bishop;
  if ((err = _bishop_new(&bishop, bishop_src->piece.id, bishop_src->piece.side, bishop_src->piece.position)) != CHESS_OK) {
    return err;
  }
  bishop->piece.is_captured = bishop_src->piece.is_captured;
  bishop->piece.moving_count = bishop_src->piece.moving_count;
  *bishop_out = bishop;
  return CHESS_OK;
}

static int _bishop_get_moves(move_array_t **move_array_out, bishop_t *bishop, board_t *board) {
  if (!(move_array_out && bishop && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(bishop->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      bool has_piece_on_position;
      if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (!has_piece_on_position) {
        move_array_add(move_array, move_new_moving_piece(bishop->piece.id, position_to));
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (piece_is_opposite(&bishop->piece, piece)) {
        move_array_add(move_array, move_new_taking_piece(bishop->piece.id, position_to, piece->id));
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

static int _bishop_free(bishop_t *bishop) {
  free(bishop);
  return CHESS_OK;
}

static int _bishop_piece_clone(piece_t **bishop_piece_out, piece_t *piece) {
  if (!(bishop_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  bishop_t *bishop, *cloned_bishop;
  if ((err = bishop_piece_cast(&bishop, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _bishop_clone(&cloned_bishop, bishop)) != CHESS_OK) {
    return err;
  }
  *bishop_piece_out = &cloned_bishop->piece;
  return CHESS_OK;
}

static int _bishop_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  bishop_t *bishop;
  if ((err = bishop_piece_cast(&bishop, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _bishop_get_moves(move_array_out, bishop, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _bishop_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  bishop_t *bishop;
  if ((err = bishop_piece_cast(&bishop, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _bishop_free(bishop)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int bishop_piece_new(piece_t **piece_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  bishop_t *bishop;
  if ((err = _bishop_new(&bishop, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_out = &bishop->piece;
  return CHESS_OK;
}

int bishop_piece_cast(bishop_t **bishop_out, piece_t *piece) {
  if (!(bishop_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_BISHOP) {
    *bishop_out = (bishop_t *)(piece - offsetof(bishop_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int board_is_position_get_attacked_by_bishop(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
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
      bishop_t *bishop;
      if ((err = bishop_piece_cast(&bishop, piece)) != CHESS_OK) {
        if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
          break;
        } else {
          return err;
        }
      }
      if (is_opposite_side(side, bishop->piece.side)) {
        *bool_out = true;
        return CHESS_OK;
      }
    }
  }
  *bool_out = false;
  return CHESS_OK;
}