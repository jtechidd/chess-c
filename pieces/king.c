#include "king.h"

#include <stdio.h>
#include <stdlib.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "rook.h"

#define KING_TOTAL_DIRECTIONS 8
const vector2_t KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                     {1, 0},  {1, -1}, {0, -1}, {-1, -1}};

void king_add_moves_positional(king_t *, board_t *, move_array_t *);
void king_add_moves_castle(king_t *, board_t *, move_array_t *,
                           move_castling_type_t);
void king_free(piece_t *piece);
move_array_t *king_get_moves(piece_t *piece, board_t *board);

king_t *king_new(piece_id_t piece_id, side_t side, vector2_t position) {
  king_t *king = (king_t *)malloc(sizeof(king_t));

  // Set piece fields
  king->piece.id = piece_id;
  king->piece.side = side;
  king->piece.type = PIECE_TYPE_KING;
  king->piece.position = position;
  king->piece.is_captured = 0;
  king->piece.moving_count = 0;

  // Set functions
  king->piece.piece_get_moves = king_get_moves;
  king->piece.piece_free = king_free;

  return king;
}

king_t *king_clone(king_t *king_src) {
  king_t *king = king_new(king_src->piece.id, king_src->piece.side,
                          king_src->piece.position);

  // Set piece fields
  king->piece.is_captured = king_src->piece.is_captured;
  king->piece.moving_count = king_src->piece.moving_count;

  return king;
}

king_t *king_cast(piece_t *piece) {
  if (piece && piece->type == PIECE_TYPE_KING) {
    return (king_t *)piece;
  }
  return NULL;
}

move_array_t *king_get_moves(piece_t *piece, board_t *board) {
  king_t *king;
  move_array_t *move_array = move_array_new();

  if (!(king = king_cast(piece))) {
    return move_array;
  }

  king_add_moves_positional(king, board, move_array);
  king_add_moves_castle(king, board, move_array, MOVE_CASTLING_TYPE_KING_SIDE);
  king_add_moves_castle(king, board, move_array, MOVE_CASTLING_TYPE_QUEEN_SIDE);

  return move_array;
}

void king_add_moves_positional(king_t *king, board_t *board,
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

rook_t *king_get_rook_to_castle_with(king_t *king, board_t *board,
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
  rook_t *rook;
  if (!(rook = rook_cast(piece_rook))) {
    return NULL;
  }
  return rook;
}

void king_add_moves_castle(king_t *king, board_t *board,
                           move_array_t *move_array,
                           move_castling_type_t move_castling_type) {
  if (board_is_king_get_attacked(board, king->piece.side)) {
    return;
  }
  rook_t *rook = king_get_rook_to_castle_with(king, board, move_castling_type);
  if (rook->piece.is_captured) {
    return;
  }
  if (!(king->piece.moving_count == 0 && rook->piece.moving_count == 0)) {
    return;
  }
  for (uint8_t j = 1; j <= 2; j++) {
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

void king_castle(piece_t *piece, move_t *move, board_t *board) {
  king_t *king;
  if (!(king = king_cast(piece))) {
    return;
  }
  if (!(move->flags & MOVE_FLAGS_HAS_CASTLING)) {
    return;
  }
  rook_t *rook = king_get_rook_to_castle_with(king, board, move->castling_type);
  if (rook->piece.is_captured) {
    return;
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
}

void king_free(piece_t *piece) {
  king_t *king;
  if (!(king = king_cast(piece))) {
    return;
  }
  free(king);
}

uint8_t board_is_position_being_attacked_by_king(board_t *board, side_t side,
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
    if (!(king = king_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, king->piece.side)) {
      return 1;
    }
  }
  return 0;
}
