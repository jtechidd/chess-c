#include "bishop.h"

#include <stdlib.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"

#define BISHOP_TOTAL_DIRECTIONS 4
const vector2_t BISHOP_DIRECTIONS[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

move_array_t *bishop_get_moves(piece_t *piece, board_t *board);
void bishop_free(piece_t *piece);

bishop_t *bishop_new(piece_id_t piece_id, side_t side, vector2_t position) {
  bishop_t *bishop = (bishop_t *)malloc(sizeof(bishop_t));

  // Set piece fields
  bishop->piece.id = piece_id;
  bishop->piece.side = side;
  bishop->piece.type = PIECE_TYPE_BISHOP;
  bishop->piece.position = position;
  bishop->piece.is_captured = 0;
  bishop->piece.moving_count = 0;

  // Set functions
  bishop->piece.piece_free = bishop_free;
  bishop->piece.piece_get_moves = bishop_get_moves;

  return bishop;
}

bishop_t *bishop_clone(bishop_t *bishop_src) {
  bishop_t *bishop = bishop_new(bishop_src->piece.id, bishop_src->piece.side,
                                bishop_src->piece.position);

  // Set piece fields
  bishop->piece.is_captured = bishop_src->piece.is_captured;
  bishop->piece.moving_count = bishop_src->piece.moving_count;

  return bishop;
}

bishop_t *bishop_cast(piece_t *piece) {
  if (!(piece && piece->type == PIECE_TYPE_BISHOP)) {
    return NULL;
  }
  return (bishop_t *)piece;
}

move_array_t *bishop_get_moves(piece_t *piece, board_t *board) {
  int err;
  bishop_t *bishop;
  move_array_t *move_array = move_array_new();

  if (!(bishop = bishop_cast(piece))) {
    return move_array;
  }

  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
    for (int scale = 1;; scale++) {
      vector2_t position_to = vector2_add2(bishop->piece.position,
                                           vector2_scaled(direction, scale));
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
                       move_new_moving_piece(bishop->piece.id, position_to));
        continue;
      }
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board, position_to)) !=
          CHESS_OK) {
        // return err;
      }
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

void bishop_free(piece_t *piece) {
  bishop_t *bishop;
  if (!(bishop = bishop_cast(piece))) {
    return;
  }
  free(bishop);
}

bool board_is_position_being_attacked_by_bishop(board_t *board, side_t side,
                                                vector2_t position) {
  int err;
  for (size_t k = 0; k < BISHOP_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = BISHOP_DIRECTIONS[k];
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
      bishop_t *bishop;
      if (!(bishop = bishop_cast(piece))) {
        break;
      }
      if (is_opposite_side(side, bishop->piece.side)) {
        return 1;
      }
    }
  }
  return 0;
}