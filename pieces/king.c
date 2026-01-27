#include "king.h"

#include <stdlib.h>

#include "../board.h"
#include "../utils.h"

#define KING_TOTAL_DIRECTIONS 8
const vector2_t KING_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

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
  king_t *king = king_new(king_src->piece.id, king_src->piece.side, king_src->piece.position);

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

  for (size_t k = 0; k < KING_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KING_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(king->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      move_array_add(move_array, move_new_moving_piece(king->piece.id, position_to));
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    if (piece_is_opposite(&king->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(king->piece.id, position_to, piece->id));
    }
  }

  // TODO: add move king castle

  return move_array;
}

void king_castle(piece_t *piece, move_t *move, board_t *board) {
  // TODO: handle king castle
}

void king_free(piece_t *piece) {
  king_t *king;
  if (!(king = king_cast(piece))) {
    return;
  }
  free(king);
}

uint8_t board_is_position_get_attacked_by_king(board_t *board, side_t side, vector2_t position) {
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
