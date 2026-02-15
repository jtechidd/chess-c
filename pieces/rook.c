#include "rook.h"

#include <stddef.h>
#include <stdlib.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define ROOK_TOTAL_DIRECTIONS 4
const vector2_t ROOK_DIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

static piece_clone_fn _rook_piece_clone;
static piece_get_moves_fn _rook_piece_get_moves;
static piece_free_fn _rook_piece_free;
static piece_get_wchar_fn _rook_piece_get_wchar;
rook_t *rook_piece_cast(piece_t *);

static rook_t *_rook_new(piece_id_t piece_id, side_t side, vector2_t position) {
  rook_t *rook = (rook_t *)malloc(sizeof(rook_t));

  rook->piece.id = piece_id;
  rook->piece.side = side;
  rook->piece.type = PIECE_TYPE_ROOK;
  rook->piece.position = position;
  rook->piece.is_captured = false;
  rook->piece.moving_count = 0;

  rook->piece.piece_clone = _rook_piece_clone;
  rook->piece.piece_get_moves = _rook_piece_get_moves;
  rook->piece.piece_free = _rook_piece_free;
  rook->piece.piece_get_wchar = _rook_piece_get_wchar;

  return rook;
}

static piece_t *_rook_piece_clone(piece_t *piece_src) {
  rook_t *rook_src = rook_piece_cast(piece_src);
  rook_t *rook = _rook_new(rook_src->piece.id, rook_src->piece.side,
                           rook_src->piece.position);

  rook->piece.is_captured = rook_src->piece.is_captured;
  rook->piece.moving_count = rook_src->piece.moving_count;

  return &rook->piece;
}

static move_array_t *_rook_piece_get_moves(piece_t *piece, board_t *board) {
  rook_t *rook = rook_piece_cast(piece);
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to =
          vector2_add2(rook->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      if (!board_has_piece_on_position(board, position_to)) {
        move_array_add(move_array,
                       move_new_moving_piece(rook->piece.id, position_to));
        continue;
      }
      piece_t *piece = board_get_piece_by_position(board, position_to);
      if (piece_is_opposite(&rook->piece, piece)) {
        move_array_add(move_array, move_new_taking_piece(
                                       rook->piece.id, position_to, piece->id));
      }
      break;
    }
  }

  return move_array;
}

static void _rook_piece_free(piece_t *piece) {
  rook_t *rook = rook_piece_cast(piece);
  xfree(rook);
}

static wchar_t _rook_piece_get_wchar(piece_t *piece) {
  rook_t *rook = rook_piece_cast(piece);
  if (rook->piece.side == SIDE_WHITE) {
    return 0x2656;
  } else if (rook->piece.side == SIDE_BLACK) {
    return 0x265C;
  }
  return '\0';
}

piece_t *rook_piece_new(piece_id_t piece_id, side_t side, vector2_t position) {
  rook_t *rook = _rook_new(piece_id, side, position);
  return &rook->piece;
}

rook_t *rook_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_ROOK) {
    return NULL;
  }
  return (rook_t *)(piece - offsetof(rook_t, piece));
}

bool board_is_position_get_attacked_by_rook(board_t *board, side_t side,
                                            vector2_t position) {
  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to =
          vector2_add2(position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      if (!board_has_piece_on_position(board, position_to)) {
        continue;
      }
      piece_t *piece = board_get_piece_by_position(board, position_to);
      rook_t *rook;
      if (!(rook = rook_piece_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, rook->piece.side)) {
        return true;
      }
    }
  }
  return false;
}