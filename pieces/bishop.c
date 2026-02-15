#include <stddef.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "bishop.h"
#include "piece.h"

#define BISHOP_TOTAL_DIRECTIONS 4
const vector2_t BISHOP_DIRECTIONS[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

static piece_clone_fn _bishop_piece_clone;
static piece_get_moves_fn _bishop_piece_get_moves;
static piece_free_fn _bishop_piece_free;
static piece_get_wchar_fn _bishop_piece_get_wchar;
bishop_t *bishop_piece_cast(piece_t *);

static bishop_t *_bishop_new(piece_id_t piece_id, side_t side,
                             vector2_t position) {
  bishop_t *bishop = (bishop_t *)xmalloc(sizeof(bishop_t));

  bishop->piece.id = piece_id;
  bishop->piece.side = side;
  bishop->piece.type = PIECE_TYPE_BISHOP;
  bishop->piece.position = position;
  bishop->piece.is_captured = 0;
  bishop->piece.moving_count = 0;

  bishop->piece.piece_clone = _bishop_piece_clone;
  bishop->piece.piece_free = _bishop_piece_free;
  bishop->piece.piece_get_moves = _bishop_piece_get_moves;
  bishop->piece.piece_get_wchar = _bishop_piece_get_wchar;

  return bishop;
}

static piece_t *_bishop_piece_clone(piece_t *piece_src) {
  bishop_t *bishop_src = bishop_piece_cast(piece_src);
  bishop_t *bishop = _bishop_new(bishop_src->piece.id, bishop_src->piece.side,
                                 bishop_src->piece.position);

  bishop->piece.is_captured = bishop_src->piece.is_captured;
  bishop->piece.moving_count = bishop_src->piece.moving_count;

  return &bishop->piece;
}

static move_array_t *_bishop_piece_get_moves(piece_t *piece, board_t *board) {
  bishop_t *bishop = bishop_piece_cast(piece);
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(bishop->piece.position,
                                           vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      if (!board_has_piece_on_position(board, position_to)) {
        move_array_add(move_array,
                       move_new_moving_piece(bishop->piece.id, position_to));
        continue;
      }
      piece_t *piece = board_get_piece_by_position(board, position_to);
      if (piece_is_opposite(&bishop->piece, piece)) {
        move_array_add(
            move_array,
            move_new_taking_piece(bishop->piece.id, position_to, piece->id));
      }
      break;
    }
  }

  return move_array;
}

static void _bishop_piece_free(piece_t *piece) {
  bishop_t *bishop = bishop_piece_cast(piece);
  xfree(bishop);
}

static wchar_t _bishop_piece_get_wchar(piece_t *piece) {
  bishop_t *bishop = bishop_piece_cast(piece);
  if (bishop->piece.side == SIDE_WHITE) {
    return 0x2657;
  } else if (bishop->piece.side == SIDE_BLACK) {
    return 0x265D;
  }
  return '\0';
}

piece_t *bishop_piece_new(piece_id_t piece_id, side_t side,
                          vector2_t position) {
  bishop_t *bishop = _bishop_new(piece_id, side, position);
  return &bishop->piece;
}

bishop_t *bishop_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_BISHOP) {
    return NULL;
  }
  return (bishop_t *)(piece - offsetof(bishop_t, piece));
}

bool board_is_position_get_attacked_by_bishop(board_t *board, side_t side,
                                              vector2_t position) {
  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
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
      bishop_t *bishop;
      if (!(bishop = bishop_piece_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, bishop->piece.side)) {
        return true;
      }
    }
  }
  return false;
}