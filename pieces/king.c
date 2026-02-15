
#include <assert.h>
#include <stddef.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "king.h"
#include "piece.h"
#include "rook.h"

#define KING_TOTAL_DIRECTIONS 8
const vector2_t KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                     {1, 0},  {1, -1}, {0, -1}, {-1, -1}};

static piece_clone_fn _king_piece_clone;
static piece_get_moves_fn _king_piece_get_moves;
static piece_free_fn _king_piece_free;
static piece_get_wchar_fn _king_piece_get_wchar;
king_t *king_piece_cast(piece_t *);

static king_t *_king_new(piece_id_t piece_id, side_t side, vector2_t position) {
  king_t *king = (king_t *)xmalloc(sizeof(king_t));

  king->piece.id = piece_id;
  king->piece.side = side;
  king->piece.type = PIECE_TYPE_KING;
  king->piece.position = position;
  king->piece.is_captured = false;
  king->piece.moving_count = 0;

  king->piece.piece_clone = _king_piece_clone;
  king->piece.piece_get_moves = _king_piece_get_moves;
  king->piece.piece_free = _king_piece_free;
  king->piece.piece_get_wchar = _king_piece_get_wchar;

  return king;
}

static piece_t *_king_piece_clone(piece_t *piece_src) {
  king_t *king_src = king_piece_cast(piece_src);
  king_t *king = _king_new(king_src->piece.id, king_src->piece.side,
                           king_src->piece.position);

  king->piece.is_captured = king_src->piece.is_captured;
  king->piece.moving_count = king_src->piece.moving_count;

  return &king->piece;
}

static void _king_add_moves_positional(king_t *king, board_t *board,
                                       move_array_t *move_array) {
  for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KING_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(king->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      move_array_add(move_array,
                     move_new_moving_piece(king->piece.id, position_to));
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    if (piece_is_opposite(&king->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(king->piece.id,
                                                       position_to, piece->id));
    }
  }
}

static rook_t *
_king_get_rook_to_castle_with(king_t *king, board_t *board,
                              move_castling_type_t move_castling_type) {
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
  piece_rook = board_get_piece_by_id(board, rook_piece_id);
  rook_t *rook = rook_piece_cast(piece_rook);
  return rook;
}

static void _king_add_moves_castle(king_t *king, board_t *board,
                                   move_array_t *move_array,
                                   move_castling_type_t move_castling_type) {
  if (board_is_king_get_attacked(board, king->piece.side)) {
    return;
  }
  rook_t *rook = _king_get_rook_to_castle_with(king, board, move_castling_type);
  if (rook->piece.is_captured) {
    return;
  }
  if (!(king->piece.moving_count == 0 && rook->piece.moving_count == 0)) {
    return;
  }
  for (int j = 1; j <= 2; j++) {
    vector2_t check_direction = vector2_make(0, j);
    if (move_castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
      check_direction = vector2_hflip(check_direction);
    }
    vector2_t check_position =
        vector2_add2(king->piece.position, check_direction);
    if (!board_is_position_safe_to_move_to(board, king->piece.side,
                                           check_position)) {
      return;
    }
  }
  move_array_add(move_array,
                 move_new_castling(king->piece.id, move_castling_type));
}

static move_array_t *_king_piece_get_moves(piece_t *piece, board_t *board) {
  king_t *king = king_piece_cast(piece);
  move_array_t *move_array = move_array_new();

  _king_add_moves_positional(king, board, move_array);
  _king_add_moves_castle(king, board, move_array, MOVE_CASTLING_TYPE_KING_SIDE);
  _king_add_moves_castle(king, board, move_array,
                         MOVE_CASTLING_TYPE_QUEEN_SIDE);

  return move_array;
}

static void _king_piece_free(piece_t *piece) {
  king_t *king = king_piece_cast(piece);
  xfree(king);
}

static wchar_t _king_piece_get_wchar(piece_t *piece) {
  king_t *king = king_piece_cast(piece);
  if (king->piece.side == SIDE_WHITE) {
    return 0x2654;
  } else if (king->piece.side == SIDE_BLACK) {
    return 0x265A;
  }
  return '\0';
}

piece_t *king_piece_new(piece_id_t piece_id, side_t side, vector2_t position) {
  king_t *king = _king_new(piece_id, side, position);
  return &king->piece;
}

king_t *king_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_KING) {
    return NULL;
  }
  return (king_t *)(piece - offsetof(king_t, piece));
}

void king_piece_castle(piece_t *piece, move_t *move, board_t *board) {
  king_t *king = king_piece_cast(piece);
  assert(king != NULL);

  rook_t *rook =
      _king_get_rook_to_castle_with(king, board, move->castling_type);

  vector2_t king_position = king->piece.position;
  vector2_t king_next = vector2_make(0, 1);
  vector2_t king_next_2 = vector2_make(0, 2);
  if (move->castling_type == MOVE_CASTLING_TYPE_QUEEN_SIDE) {
    king_next = vector2_hflip(king_next);
    king_next_2 = vector2_hflip(king_next_2);
  }
  king->piece.position = vector2_add2(king_position, king_next_2);
  rook->piece.position = vector2_add2(king_position, king_next);
}

bool board_is_position_being_attacked_by_king(board_t *board, side_t side,
                                              vector2_t position) {
  for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KING_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    king_t *king;
    if (!(king = king_piece_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, king->piece.side)) {
      return 1;
    }
  }
  return 0;
}
