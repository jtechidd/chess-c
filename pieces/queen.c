#include "queen.h"

#include <stdlib.h>

#include "../board.h"
#include "../move/move_array.h"
#include "../utils.h"

#define QUEEN_TOTAL_DIRECTIONS 8
const vector2_t QUEEN_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1},  {1, 1},
                                      {1, 0},  {1, -1}, {0, -1}, {-1, -1}};

move_array_t *queen_get_moves(piece_t *piece, board_t *board);
void queen_free(piece_t *piece);

queen_t *queen_new(piece_id_t piece_id, side_t side, vector2_t position) {
  queen_t *queen = (queen_t *)malloc(sizeof(queen_t));

  // Set piece fields
  queen->piece.id = piece_id;
  queen->piece.side = side;
  queen->piece.type = PIECE_TYPE_QUEEN;
  queen->piece.position = position;
  queen->piece.is_captured = 0;
  queen->piece.moving_count = 0;

  // Set functions
  queen->piece.piece_get_moves = queen_get_moves;
  queen->piece.piece_free = queen_free;

  return queen;
}

queen_t *queen_clone(queen_t *queen_src) {
  queen_t *queen = queen_new(queen_src->piece.id, queen_src->piece.side,
                             queen_src->piece.position);

  // Set piece fields
  queen->piece.is_captured = queen_src->piece.is_captured;
  queen->piece.moving_count = queen_src->piece.moving_count;

  return queen;
}

queen_t *queen_cast(piece_t *piece) {
  if (piece && piece->type == PIECE_TYPE_QUEEN) {
    return (queen_t *)piece;
  }
  return NULL;
}

move_array_t *queen_get_moves(piece_t *piece, board_t *board) {
  queen_t *queen;
  move_array_t *move_array = move_array_new();

  if (!(queen = queen_cast(piece))) {
    return move_array;
  }

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

void queen_free(piece_t *piece) {
  queen_t *queen;
  if (!(queen = queen_cast(piece))) {
    return;
  }
  free(queen);
}

bool board_is_position_being_attacked_by_queen(board_t *board, side_t side,
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
      if (!(queen = queen_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, queen->piece.side)) {
        return 1;
      }
    }
  }
  return 0;
}