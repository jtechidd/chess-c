#include "pawn.h"

#include <stdlib.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "bishop.h"
#include "knight.h"
#include "queen.h"
#include "rook.h"

#define PAWN_UP_TOTAL_DIRECTIONS 2
const vector2_t PAWN_UP_DIRECTIONS[] = {{-1, 0}, {-2, 0}};
#define PAWN_TAKE_TOTAL_DIRECTIONS 2
const vector2_t PAWN_TAKE_DIRECTIONS[] = {{-1, -1}, {-1, 1}};

void pawn_add_moves_up(pawn_t *, board_t *, move_array_t *);
void pawn_add_moves_take(pawn_t *, board_t *, move_array_t *);
void pawn_add_moves_en_passant(pawn_t *, board_t *, move_array_t *);
void pawn_add_move(pawn_t *, move_array_t *, move_t *);
void pawn_add_promotion_moves(move_array_t *, move_t *);

pawn_t *pawn_new(piece_id_t piece_id, side_t side, vector2_t position) {
  pawn_t *pawn = (pawn_t *)malloc(sizeof(pawn_t));

  // Set piece fields
  pawn->piece.id = piece_id;
  pawn->piece.side = side;
  pawn->piece.type = PIECE_TYPE_PAWN;
  pawn->piece.position = position;
  pawn->piece.is_captured = 0;
  pawn->piece.moving_count = 0;

  // Set functions
  pawn->piece.piece_get_moves = pawn_get_moves;
  pawn->piece.piece_free = pawn_free;

  // Set fields
  pawn->can_get_en_passant = 0;

  return pawn;
}

pawn_t *pawn_clone(pawn_t *pawn_src) {
  pawn_t *pawn = pawn_new(pawn_src->piece.id, pawn_src->piece.side, pawn_src->piece.position);

  // Set piece fields
  pawn->piece.is_captured = pawn_src->piece.is_captured;
  pawn->piece.moving_count = pawn_src->piece.moving_count;

  // Set fields
  pawn->can_get_en_passant = pawn_src->can_get_en_passant;

  return pawn;
}

pawn_t *pawn_cast(piece_t *piece) {
  if (!(piece && piece->type == PIECE_TYPE_PAWN)) {
    return NULL;
  }
  return (pawn_t *)piece;
}

move_array_t *pawn_get_moves(piece_t *piece, board_t *board) {
  pawn_t *pawn;
  move_array_t *move_array = move_array_new();

  if (!(pawn = pawn_cast(piece))) {
    return move_array;
  }

  pawn_add_moves_up(pawn, board, move_array);
  pawn_add_moves_take(pawn, board, move_array);
  pawn_add_moves_en_passant(pawn, board, move_array);

  return move_array;
}

void pawn_add_moves_up(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;

  size_t total_directions = PAWN_UP_TOTAL_DIRECTIONS;

  if (pawn->piece.moving_count > 0) {
    total_directions--;
  }

  for (size_t k = 0; k < total_directions; k++) {
    vector2_t direction = PAWN_UP_DIRECTIONS[k];
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(pawn->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      break;
    }
    if (board_has_piece_on_position(board, position_to)) {
      break;
    }
    move_t *move = move_new_moving_piece(pawn->piece.id, position_to);
    pawn_add_move(pawn, move_array, move);
  }
}

void pawn_add_moves_take(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;

  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(pawn->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_can_take_position(board, &pawn->piece, position_to)) {
      continue;
    }
    piece_t *take_piece = board_get_piece_by_position(board, position_to);
    move_t *move = move_new_taking_piece(pawn->piece.id, position_to, take_piece->id);
    pawn_add_move(pawn, move_array, move);
  }
}

void pawn_add_moves_en_passant(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;

  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    vector2_t direction_side_pawn = vector2_make(0, direction.j);
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(pawn->piece.position, direction);
    vector2_t position_side_pawn = vector2_add2(pawn->piece.position, direction_side_pawn);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!is_position_in_bound(position_side_pawn)) {
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_side_pawn);
    pawn_t *side_pawn;
    if (!(side_pawn = pawn_cast(piece))) {
      return;
    }
    if (!piece_is_opposite(&pawn->piece, piece)) {
      return;
    }
    if (!side_pawn->can_get_en_passant) {
      return;
    }
    move_t *move = move_new_taking_piece(pawn->piece.id, position_to, side_pawn->piece.id);
    pawn_add_move(pawn, move_array, move);
  }
}

// Relaying to add single move or to add promotion moves
void pawn_add_move(pawn_t *pawn, move_array_t *move_array, move_t *move) {
  side_t side = pawn->piece.side;

  uint8_t can_be_promoted = 0;
  if (side == SIDE_WHITE) {
    can_be_promoted = is_position_top(move->position_to);
  } else if (side == SIDE_BLACK) {
    can_be_promoted = is_position_bottom(move->position_to);
  }

  if (can_be_promoted) {
    pawn_add_promotion_moves(move_array, move);
  } else {
    move_array_add(move_array, move);
  }
}

// Clone move and configured with promotable piece, then add to array
void pawn_add_promotion_moves(move_array_t *move_array, move_t *move) {
  move_t *cloned_move = move_clone(move);
  move_with_promotion(cloned_move, PIECE_TYPE_QUEEN);
  move_array_add(move_array, cloned_move);

  cloned_move = move_clone(move);
  move_with_promotion(cloned_move, PIECE_TYPE_ROOK);
  move_array_add(move_array, cloned_move);

  cloned_move = move_clone(move);
  move_with_promotion(cloned_move, PIECE_TYPE_BISHOP);
  move_array_add(move_array, cloned_move);

  cloned_move = move_clone(move);
  move_with_promotion(cloned_move, PIECE_TYPE_KNIGHT);
  move_array_add(move_array, cloned_move);
}

void pawn_flag_can_get_en_passant(piece_t *piece, move_t *move) {
  pawn_t *pawn;
  if (!(pawn = pawn_cast(piece))) {
    return;
  }
  if (!(move->flags & MOVE_FLAGS_HAS_MOVING_PIECE)) {
    return;
  }
  if (pawn->piece.id != move->piece_id) {
    return;
  }

  if (pawn->piece.moving_count > 0) {
    if (pawn->can_get_en_passant) {
      pawn->can_get_en_passant = 0;
    }
    return;
  }
  if (vector2_l1dist(pawn->piece.position, move->position_to) == 2) {
    pawn->can_get_en_passant = 1;
  }
}

void pawn_promote(piece_t *piece, move_t *move, board_t *board) {
  pawn_t *pawn;
  if (!(pawn = pawn_cast(piece))) {
    return;
  }
  if (!((move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) && (move->flags & MOVE_FLAGS_HAS_PROMOTION))) {
    return;
  }
  if (piece->id != move->piece_id) {
    return;
  }

  switch (move->promote_to) {
    case PIECE_TYPE_QUEEN:
      board_register_piece(board, (piece_t *)queen_new(pawn->piece.id, piece->side, move->position_to));
      break;
    case PIECE_TYPE_ROOK:
      board_register_piece(board, (piece_t *)rook_new(pawn->piece.id, piece->side, move->position_to));
      break;
    case PIECE_TYPE_BISHOP:
      board_register_piece(board, (piece_t *)bishop_new(pawn->piece.id, piece->side, move->position_to));
      break;
    case PIECE_TYPE_KNIGHT:
      board_register_piece(board, (piece_t *)knight_new(pawn->piece.id, piece->side, move->position_to));
      break;
    default:
      break;
  }
  piece->piece_free(piece);
}

void pawn_free(piece_t *piece) {
  pawn_t *pawn;
  if (!(pawn = pawn_cast(piece))) {
    return;
  }
  free(pawn);
}

uint8_t board_is_position_get_attacked_by_pawn(board_t *board, side_t side, vector2_t position) {
  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!board_has_piece_on_position(board, position_to)) {
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_to);
    pawn_t *pawn;
    if (!(pawn = pawn_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, pawn->piece.side)) {
      return 1;
    }
  }
  return 0;
}