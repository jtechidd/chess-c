#include "queen.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"

#define QUEEN_TOTAL_DIRECTIONS 8
const vector2_t QUEEN_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                      {1, 0},  {1, -1}, {0, -1}, {-1, -1}};

static piece_clone_fn _queen_piece_clone;
static piece_get_moves_fn _queen_piece_get_moves;
static piece_free_fn _queen_piece_free;
static piece_get_wchar_fn _queen_piece_get_wchar;
queen_t *queen_piece_cast(piece_t *);

static queen_t *_queen_new(piece_id_t piece_id, side_t side,
                           vector2_t position) {
  queen_t *queen = (queen_t *)xmalloc(sizeof(queen_t));

  queen->piece.id = piece_id;
  queen->piece.side = side;
  queen->piece.type = PIECE_TYPE_QUEEN;
  queen->piece.position = position;
  queen->piece.is_captured = false;
  queen->piece.moving_count = 0;

  queen->piece.piece_clone = _queen_piece_clone;
  queen->piece.piece_get_moves = _queen_piece_get_moves;
  queen->piece.piece_free = _queen_piece_free;
  queen->piece.piece_get_wchar = _queen_piece_get_wchar;

  return queen;
}

static piece_t *_queen_piece_clone(piece_t *piece_src) {
  queen_t *queen_src = queen_piece_cast(piece_src);
  queen_t *queen = _queen_new(queen_src->piece.id, queen_src->piece.side,
                              queen_src->piece.position);

  queen->piece.is_captured = queen_src->piece.is_captured;
  queen->piece.moving_count = queen_src->piece.moving_count;

  return &queen->piece;
}

static move_array_t *_queen_piece_get_moves(piece_t *piece, board_t *board) {
  queen_t *queen = queen_piece_cast(piece);
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = QUEEN_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to =
          vector2_add2(queen->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      if (!board_has_piece_on_position(board, position_to)) {
        move_array_add(move_array,
                       move_new_moving_piece(queen->piece.id, position_to));
        continue;
      }
      piece_t *piece = board_get_piece_by_position(board, position_to);
      if (piece_is_opposite(&queen->piece, piece)) {
        move_array_add(
            move_array,
            move_new_taking_piece(queen->piece.id, position_to, piece->id));
      }
      break;
    }
  }
  return move_array;
}

static void _queen_piece_free(piece_t *piece) {
  queen_t *queen = queen_piece_cast(piece);
  xfree(queen);
}

static wchar_t _queen_piece_get_wchar(piece_t *piece) {
  queen_t *queen = queen_piece_cast(piece);
  if (queen->piece.side == SIDE_WHITE) {
    return 0x2655;
  } else if (queen->piece.side == SIDE_BLACK) {
    return 0x265B;
  }
  return '\0';
}

piece_t *queen_piece_new(piece_id_t piece_id, side_t side, vector2_t position) {
  queen_t *queen = _queen_new(piece_id, side, position);
  return &queen->piece;
}

queen_t *queen_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_QUEEN) {
    return NULL;
  }
  return (queen_t *)(piece - offsetof(queen_t, piece));
}

bool board_is_position_get_attacked_by_queen(board_t *board, side_t side,
                                             vector2_t position) {
  for (size_t k = 0; k < QUEEN_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = QUEEN_DIRECTIONS[k];
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
      queen_t *queen;
      if (!(queen = queen_piece_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, queen->piece.side)) {
        return true;
      }
    }
  }
  return false;
}