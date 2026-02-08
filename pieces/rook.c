#include "rook.h"

#include <stdlib.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"

#define ROOK_TOTAL_DIRECTIONS 4
const vector2_t ROOK_DIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

move_array_t *rook_get_moves(piece_t *piece, board_t *board);
void rook_free(piece_t *piece);

rook_t *rook_new(piece_id_t piece_id, side_t side, vector2_t position) {
  rook_t *rook = (rook_t *)malloc(sizeof(rook_t));

  // Set piece fields
  rook->piece.id = piece_id;
  rook->piece.side = side;
  rook->piece.type = PIECE_TYPE_ROOK;
  rook->piece.position = position;
  rook->piece.is_captured = 0;
  rook->piece.moving_count = 0;

  // Set piece functions
  rook->piece.piece_get_moves = rook_get_moves;
  rook->piece.piece_free = rook_free;

  return rook;
}

rook_t *rook_clone(rook_t *rook_src) {
  rook_t *rook = rook_new(rook_src->piece.id, rook_src->piece.side,
                          rook_src->piece.position);

  // Set piece fields
  rook->piece.is_captured = rook_src->piece.is_captured;
  rook->piece.moving_count = rook_src->piece.moving_count;

  return rook;
}

rook_t *rook_cast(piece_t *piece) {
  if (piece && piece->type == PIECE_TYPE_ROOK) {
    return (rook_t *)piece;
  }
  return NULL;
}

move_array_t *rook_get_moves(piece_t *piece, board_t *board) {
  int err;
  rook_t *rook;
  move_array_t *move_array = move_array_new();

  if (!(rook = rook_cast(piece))) {
    return move_array;
  }

  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to =
          vector2_add2(rook->piece.position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      bool has_piece_on_position;
      if ((err = board_has_piece_on_position(&has_piece_on_position, board,
                                             position_to)) != CHESS_OK) {
        // return err;
      }
      if (!has_piece_on_position) {
        move_array_add(move_array,
                       move_new_moving_piece(rook->piece.id, position_to));
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) !=
          CHESS_OK) {
        // return err;
      }
      if (piece_is_opposite(&rook->piece, piece)) {
        move_array_add(move_array, move_new_taking_piece(
                                       rook->piece.id, position_to, piece->id));
      }
      break;
    }
  }

  return move_array;
}

void rook_free(piece_t *piece) {
  rook_t *rook;
  if (!(rook = rook_cast(piece))) {
    return;
  }
  free(rook);
}

bool board_is_position_being_attacked_by_rook(board_t *board, side_t side,
                                              vector2_t position) {
  int err;
  for (size_t k = 0; k < ROOK_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = ROOK_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to =
          vector2_add2(position, vector2_scaled(direction, scale));
      if (!is_position_in_bound(position_to)) {
        break;
      }
      bool has_piece_on_position;
      if ((err = board_has_piece_on_position(&has_piece_on_position, board,
                                             position_to)) != CHESS_OK) {
        // return err;
      }
      if (!has_piece_on_position) {
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) !=
          CHESS_OK) {
        // return err;
      }
      rook_t *rook;
      if (!(rook = rook_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, rook->piece.side)) {
        return 1;
      }
    }
  }
  return 0;
}