#include "queen.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define QUEEN_TOTAL_DIRECTIONS 8
#define QUEEN_MAX_SCALE 7
const vector2_t QUEEN_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

static piece_clone_fn _queen_piece_clone;
static piece_get_moves_fn _queen_piece_get_moves;
static piece_free_fn _queen_piece_free;

static int _queen_new(queen_t **queen_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(queen_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }

  queen_t *queen = (queen_t *)malloc(sizeof(queen_t));
  if (!queen) {
    return CHESS_ERROR_NO_MEMORY;
  }

  queen->piece.id = piece_id;
  queen->piece.side = side;
  queen->piece.type = PIECE_TYPE_QUEEN;
  queen->piece.position = position;
  queen->piece.is_captured = false;
  queen->piece.moving_count = 0;

  queen->piece.piece_clone = _queen_piece_clone;
  queen->piece.piece_free = _queen_piece_free;
  queen->piece.piece_get_moves = _queen_piece_get_moves;

  *queen_out = queen;
  return CHESS_OK;
}

static int _queen_clone(queen_t **queen_out, queen_t *queen_src) {
  if (!(queen_out && queen_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  queen_t *queen;
  if ((err = _queen_new(&queen, queen_src->piece.id, queen_src->piece.side, queen_src->piece.position)) != CHESS_OK) {
    return err;
  }
  queen->piece.is_captured = queen_src->piece.is_captured;
  queen->piece.moving_count = queen_src->piece.moving_count;
  *queen_out = queen;
  return CHESS_OK;
}

static int _queen_get_moves(move_array_t **move_array_out, queen_t *queen, board_t *board) {
  if (!(move_array_out && queen && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = QUEEN_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(queen->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      bool has_piece_on_position;
      if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (!has_piece_on_position) {
        move_array_add(move_array, move_new_moving_piece(queen->piece.id, position_to));
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
        goto fail;
      }
      if (piece_is_opposite(&queen->piece, piece)) {
        move_array_add(move_array, move_new_taking_piece(queen->piece.id, position_to, piece->id));
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

static int _queen_free(queen_t *queen) {
  free(queen);
  return CHESS_OK;
}

static int _queen_piece_clone(piece_t **queen_piece_out, piece_t *piece) {
  if (!(queen_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  queen_t *queen, *cloned_queen;
  if ((err = queen_piece_cast(&queen, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _queen_clone(&cloned_queen, queen)) != CHESS_OK) {
    return err;
  }
  *queen_piece_out = &cloned_queen->piece;
  return CHESS_OK;
}

static int _queen_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  queen_t *queen;
  if ((err = queen_piece_cast(&queen, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _queen_get_moves(move_array_out, queen, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _queen_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  queen_t *queen;
  if ((err = queen_piece_cast(&queen, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _queen_free(queen)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int queen_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  queen_t *queen;
  if ((err = _queen_new(&queen, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_t = &queen->piece;
  return CHESS_OK;
}

int queen_piece_cast(queen_t **queen_out, piece_t *piece) {
  if (!(queen_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_QUEEN) {
    *queen_out = (queen_t *)(piece - offsetof(queen_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int board_is_position_get_attacked_by_queen(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = QUEEN_DIRECTIONS[k];
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
      queen_t *queen;
      if ((err = queen_piece_cast(&queen, piece)) != CHESS_OK) {
        if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
          break;
        } else {
          return err;
        }
      }
      if (is_opposite_side(side, queen->piece.side)) {
        *bool_out = true;
        return CHESS_OK;
      }
    }
  }
  *bool_out = false;
  return CHESS_OK;
}