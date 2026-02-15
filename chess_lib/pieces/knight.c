
#include <stddef.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "knight.h"
#include "piece.h"

#define KNIGHT_TOTAL_DIRECTIONS 8
const vector2_t KNIGHT_DIRECTIONS[] = {{-2, -1}, {-2, 1}, {-1, 2},  {1, 2},
                                       {2, -1},  {2, 1},  {-1, -2}, {1, -2}};

static piece_clone_fn _knight_piece_clone;
static piece_get_moves_fn _knight_piece_get_moves;
static piece_free_fn _knight_piece_free;
static piece_get_wchar_fn _knight_piece_get_wchar;
knight_t *knight_piece_cast(piece_t *);

static knight_t *_knight_new(piece_id_t piece_id, side_t side,
                             vector2_t position) {
  knight_t *knight = (knight_t *)xmalloc(sizeof(knight_t));

  knight->piece.id = piece_id;
  knight->piece.side = side;
  knight->piece.type = PIECE_TYPE_KNIGHT;
  knight->piece.position = position;
  knight->piece.is_captured = false;
  knight->piece.moving_count = 0;

  knight->piece.piece_clone = _knight_piece_clone;
  knight->piece.piece_get_moves = _knight_piece_get_moves;
  knight->piece.piece_free = _knight_piece_free;
  knight->piece.piece_get_wchar = _knight_piece_get_wchar;

  return knight;
}

static piece_t *_knight_piece_clone(piece_t *piece_src) {
  knight_t *knight_src = knight_piece_cast(piece_src);
  knight_t *knight = _knight_new(knight_src->piece.id, knight_src->piece.side,
                                 knight_src->piece.position);

  knight->piece.is_captured = knight_src->piece.is_captured;
  knight->piece.moving_count = knight_src->piece.moving_count;

  return &knight->piece;
}

static move_array_t *_knight_piece_get_moves(piece_t *piece, board_t *board) {
  knight_t *knight = knight_piece_cast(piece);
  move_array_t *move_array = move_array_new();
  for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KNIGHT_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(knight->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      move_array_add(move_array,
                     move_new_moving_piece(knight->piece.id, position_to));
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    if (piece_is_opposite(&knight->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(knight->piece.id,
                                                       position_to, piece->id));
    }
  }
  return move_array;
}

static void _knight_piece_free(piece_t *piece) {
  knight_t *knight = knight_piece_cast(piece);
  xfree(knight);
}

static wchar_t _knight_piece_get_wchar(piece_t *piece) {
  knight_t *knight = knight_piece_cast(piece);
  if (knight->piece.side == SIDE_WHITE) {
    return 0x2658;
  } else if (knight->piece.side == SIDE_BLACK) {
    return 0x265E;
  }
  return '\0';
}

piece_t *knight_piece_new(piece_id_t piece_id, side_t side,
                          vector2_t position) {
  knight_t *knight = _knight_new(piece_id, side, position);
  return &knight->piece;
}

knight_t *knight_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_KNIGHT) {
    return NULL;
  }
  return (knight_t *)(piece - offsetof(knight_t, piece));
}

bool board_is_position_get_attacked_by_knight(board_t *board, side_t side,
                                              vector2_t position) {
  for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KNIGHT_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    knight_t *knight;
    if (!(knight = knight_piece_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, knight->piece.side)) {
      return true;
    }
  }
  return false;
}