#include "knight.h"

#include <stdlib.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"

#define KNIGHT_TOTAL_DIRECTIONS 8
const vector2_t KNIGHT_DIRECTIONS[] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}};

move_array_t *knight_get_moves(piece_t *piece, board_t *board);
void knight_free(piece_t *piece);

knight_t *knight_new(piece_id_t piece_id, side_t side, vector2_t position) {
  knight_t *knight = (knight_t *)malloc(sizeof(knight_t));

  // Set piece fields
  knight->piece.id = piece_id;
  knight->piece.side = side;
  knight->piece.type = PIECE_TYPE_KNIGHT;
  knight->piece.position = position;
  knight->piece.is_captured = 0;
  knight->piece.moving_count = 0;

  // Set functions
  knight->piece.piece_get_moves = knight_get_moves;
  knight->piece.piece_free = knight_free;

  return knight;
}

knight_t *knight_clone(knight_t *knight_src) {
  knight_t *knight = knight_new(knight_src->piece.id, knight_src->piece.side, knight_src->piece.position);

  // Set piece fields
  knight->piece.is_captured = knight->piece.is_captured;
  knight->piece.moving_count = knight->piece.moving_count;

  return knight;
}

knight_t *knight_cast(piece_t *piece) {
  if (piece && piece->type == PIECE_TYPE_KNIGHT) {
    return (knight_t *)piece;
  }
  return NULL;
}

move_array_t *knight_get_moves(piece_t *piece, board_t *board) {
  knight_t *knight;
  move_array_t *move_array = move_array_new();

  if (!(knight = knight_cast(piece))) {
    return move_array;
  }

  for (size_t k = 0; k < KNIGHT_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = KNIGHT_DIRECTIONS[k];
    vector2_t position_to = vector2_add2(knight->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      move_array_add(move_array, move_new_moving_piece(knight->piece.id, position_to));
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    if (piece_is_opposite(&knight->piece, piece)) {
      move_array_add(move_array, move_new_taking_piece(knight->piece.id, position_to, piece->id));
    }
  }
  return move_array;
}

void knight_free(piece_t *piece) {
  knight_t *knight;
  if (!(knight = knight_cast(piece))) {
    return;
  }
  free(knight);
}

uint8_t board_is_position_get_attacked_by_knight(board_t *board, side_t side, vector2_t position) {
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
    if (!(knight = knight_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, knight->piece.side)) {
      return 1;
    }
  }
  return 0;
}