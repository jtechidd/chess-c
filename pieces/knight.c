#include "knight.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define KNIGHT_TOTAL_DIRECTIONS 8
const vector2_t KNIGHT_DIRECTIONS[] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}};

static piece_clone_fn _knight_piece_clone;
static piece_get_moves_fn _knight_piece_get_moves;
static piece_free_fn _knight_piece_free;

static int _knight_new(knight_t **knight_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(knight_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }

  knight_t *knight = (knight_t *)malloc(sizeof(knight_t));
  if (!knight) {
    return CHESS_ERROR_NO_MEMORY;
  }

  knight->piece.id = piece_id;
  knight->piece.side = side;
  knight->piece.type = PIECE_TYPE_KNIGHT;
  knight->piece.position = position;
  knight->piece.is_captured = false;
  knight->piece.moving_count = 0;

  knight->piece.piece_clone = _knight_piece_clone;
  knight->piece.piece_free = _knight_piece_free;
  knight->piece.piece_get_moves = _knight_piece_get_moves;

  *knight_out = knight;
  return CHESS_OK;
}

static int _knight_clone(knight_t **knight_out, knight_t *knight_src) {
  if (!(knight_out && knight_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  knight_t *knight;
  if ((err = _knight_new(&knight, knight_src->piece.id, knight_src->piece.side, knight_src->piece.position)) != CHESS_OK) {
    return err;
  }
  knight->piece.is_captured = knight_src->piece.is_captured;
  knight->piece.moving_count = knight_src->piece.moving_count;
  *knight_out = knight;
  return CHESS_OK;
}

static int _knight_get_moves(move_array_t **move_array_out, knight_t *knight, board_t *board) {
  if (!(move_array_out && knight && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KNIGHT_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(knight->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      break;
    }
    bool has_piece_on_position;
    if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
      goto fail;
    }
    if (!has_piece_on_position) {
      move_array_add(move_array, move_new_moving_piece(knight->piece.id, position_to));
      continue;
    }
    piece_t *piece;
    if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
      goto fail;
    }
    if (piece_is_opposite(&knight->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(knight->piece.id, position_to, piece->id));
    }
    break;
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

static int _knight_free(knight_t *knight) {
  free(knight);
  return CHESS_OK;
}

static int _knight_piece_clone(piece_t **knight_piece_out, piece_t *piece) {
  if (!(knight_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  knight_t *knight, *cloned_knight;
  if ((err = knight_piece_cast(&knight, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _knight_clone(&cloned_knight, knight)) != CHESS_OK) {
    return err;
  }
  *knight_piece_out = &cloned_knight->piece;
  return CHESS_OK;
}

static int _knight_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return -2;
  }
  int err;
  knight_t *knight;
  if ((err = knight_piece_cast(&knight, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _knight_get_moves(move_array_out, knight, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _knight_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  knight_t *knight;
  if ((err = knight_piece_cast(&knight, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _knight_free(knight)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int knight_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  knight_t *knight;
  if ((err = _knight_new(&knight, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_t = &knight->piece;
  return CHESS_OK;
}

int knight_piece_cast(knight_t **knight_out, piece_t *piece) {
  if (!(knight_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_KNIGHT) {
    *knight_out = (knight_t *)(piece - offsetof(knight_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int board_is_position_get_attacked_by_knight(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KNIGHT_DIRECTIONS[k];
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
    knight_t *knight;
    if ((err = knight_piece_cast(&knight, piece)) != CHESS_OK) {
      if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
        continue;
      } else {
        return err;
      }
    }
    if (is_opposite_side(side, knight->piece.side)) {
      *bool_out = true;
      return CHESS_OK;
    }
  }
  *bool_out = false;
  return CHESS_OK;
}
