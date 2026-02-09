#include "pawn.h"

#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "bishop.h"
#include "knight.h"
#include "piece.h"
#include "queen.h"
#include "rook.h"

#define PAWN_UP_TOTAL_DIRECTIONS 2
const vector2_t PAWN_UP_DIRECTIONS[] = {{-1, 0}, {-2, 0}};
#define PAWN_TAKE_TOTAL_DIRECTIONS 2
const vector2_t PAWN_TAKE_DIRECTIONS[] = {{-1, -1}, {-1, 1}};

static piece_clone_fn _pawn_piece_clone;
static piece_get_moves_fn _pawn_piece_get_moves;
static piece_free_fn _pawn_piece_free;

static int _pawn_new(pawn_t **pawn_out, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(pawn_out && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }

  pawn_t *pawn = (pawn_t *)malloc(sizeof(pawn_t));
  if (!pawn) {
    return CHESS_ERROR_NO_MEMORY;
  }

  pawn->piece.id = piece_id;
  pawn->piece.side = side;
  pawn->piece.type = PIECE_TYPE_PAWN;
  pawn->piece.position = position;
  pawn->piece.is_captured = false;
  pawn->piece.moving_count = 0;

  pawn->piece.piece_clone = _pawn_piece_clone;
  pawn->piece.piece_get_moves = _pawn_piece_get_moves;
  pawn->piece.piece_free = _pawn_piece_free;

  pawn->can_get_en_passant = 0;

  *pawn_out = pawn;
  return CHESS_OK;
}

static int _pawn_clone(pawn_t **pawn_out, pawn_t *pawn_src) {
  if (!(pawn_out && pawn_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn;
  if ((err = _pawn_new(&pawn, pawn_src->piece.id, pawn_src->piece.side, pawn_src->piece.position)) != CHESS_OK) {
    return err;
  }
  pawn->piece.is_captured = pawn_src->piece.is_captured;
  pawn->piece.moving_count = pawn_src->piece.moving_count;
  pawn->can_get_en_passant = pawn_src->can_get_en_passant;
  *pawn_out = pawn;
  return CHESS_OK;
}

static int _pawn_add_promotion_moves(move_array_t *move_array, move_t *move) {
  if (!(move_array && move)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
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
  return CHESS_OK;
}

static int _pawn_add_move(pawn_t *pawn, move_array_t *move_array, move_t *move) {
  if (!(pawn && move_array && move)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  side_t side = pawn->piece.side;

  bool can_be_promoted = false;
  if (side == SIDE_WHITE) {
    can_be_promoted = is_position_top(move->position_to);
  } else if (side == SIDE_BLACK) {
    can_be_promoted = is_position_bottom(move->position_to);
  }

  if (can_be_promoted) {
    if ((err = _pawn_add_promotion_moves(move_array, move)) != CHESS_OK) {
      return err;
    }
  } else {
    move_array_add(move_array, move);
  }
  return CHESS_OK;
}

static int _pawn_add_moves_up(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  if (!(pawn && board && move_array)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;
  move_t *move;

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
    bool has_piece_on_position;
    if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
      goto fail;
    }
    if (has_piece_on_position) {
      break;
    }
    move = move_new_moving_piece(pawn->piece.id, position_to);
    if ((err = _pawn_add_move(pawn, move_array, move)) != CHESS_OK) {
      goto fail;
    }
  }
  return CHESS_OK;
fail:
  if (move != NULL) {
    move_free(move);
    move = NULL;
  }
  return err;
}

static int _pawn_add_moves_take(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  if (!(pawn && board && move_array)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;
  move_t *move;

  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(pawn->piece.position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    bool can_take_position;
    if ((err = board_can_take_position(&can_take_position, board, &pawn->piece, position_to)) != CHESS_OK) {
      goto fail;
    }
    if (!can_take_position) {
      continue;
    }
    piece_t *take_piece;
    if ((err = board_get_piece_by_position(&take_piece, board, position_to)) != CHESS_OK) {
      goto fail;
    }
    move_t *move = move_new_taking_piece(pawn->piece.id, position_to, take_piece->id);
    if ((err = _pawn_add_move(pawn, move_array, move)) != CHESS_OK) {
      goto fail;
    }
  }
  return CHESS_OK;
fail:
  if (move != NULL) {
    move_free(move);
    move = NULL;
  }
  return err;
}

static int _pawn_add_moves_en_passant(pawn_t *pawn, board_t *board, move_array_t *move_array) {
  if (!(pawn && board && move_array)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;
  move_t *move;

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
    piece_t *piece;
    if ((err = board_get_piece_by_position(&piece, board, position_side_pawn)) != CHESS_OK) {
      goto fail;
    }
    pawn_t *side_pawn;
    if ((err = pawn_piece_cast(&side_pawn, piece))) {
      if (err == CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH) {
        return CHESS_OK;
      } else {
        goto fail;
      }
    }
    if (!piece_is_opposite(&pawn->piece, piece)) {
      return CHESS_OK;
    }
    if (!side_pawn->can_get_en_passant) {
      return CHESS_OK;
    }
    move = move_new_taking_piece(pawn->piece.id, position_to, side_pawn->piece.id);
    if ((err = _pawn_add_move(pawn, move_array, move)) != CHESS_OK) {
      goto fail;
    }
  }
  return CHESS_OK;
fail:
  if (move != NULL) {
    move_free(move);
    move = NULL;
  }
  return err;
}

static int _pawn_get_moves(move_array_t **move_array_out, pawn_t *pawn, board_t *board) {
  if (!(move_array_out && pawn && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  move_array_t *move_array = move_array_new();
  if ((err = _pawn_add_moves_up(pawn, board, move_array)) != CHESS_OK) {
    goto fail;
  }
  if ((err = _pawn_add_moves_take(pawn, board, move_array)) != CHESS_OK) {
    goto fail;
  }
  if ((err = _pawn_add_moves_en_passant(pawn, board, move_array)) != CHESS_OK) {
    goto fail;
  }
  *move_array_out = move_array;
  return CHESS_OK;
fail:
  if (move_array != NULL) {
    move_array_free(move_array);
    move_array = NULL;
  }
  return err;
}

static int _pawn_free(pawn_t *pawn) {
  free(pawn);
  return CHESS_OK;
}

static int _pawn_flag_can_get_en_passant(pawn_t *pawn, move_t *move) {
  if (!(pawn && move)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (!(move->flags & MOVE_FLAGS_HAS_MOVING_PIECE)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (pawn->piece.id != move->piece_id) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  if (pawn->piece.moving_count > 0) {
    if (pawn->can_get_en_passant) {
      pawn->can_get_en_passant = false;
    }
    return CHESS_OK;
  }
  if (vector2_l1dist(pawn->piece.position, move->position_to) == 2) {
    pawn->can_get_en_passant = true;
  }
  return CHESS_OK;
}

static int _pawn_promote(pawn_t *pawn, move_t *move, board_t *board) {
  if (!(pawn && move && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (!((move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) && (move->flags & MOVE_FLAGS_HAS_PROMOTION))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (pawn->piece.id != move->piece_id) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_t *piece_promote = NULL;
  switch (move->promote_to) {
  case PIECE_TYPE_QUEEN:
    if ((err = queen_piece_new(&piece_promote, pawn->piece.id, pawn->piece.side, move->position_to)) != CHESS_OK) {
      goto fail;
    }
    break;
  case PIECE_TYPE_ROOK:
    if ((err = rook_piece_new(&piece_promote, pawn->piece.id, pawn->piece.side, move->position_to)) != CHESS_OK) {
      goto fail;
    }
    break;
  case PIECE_TYPE_BISHOP:
    if ((err = bishop_piece_new(&piece_promote, pawn->piece.id, pawn->piece.side, move->position_to)) != CHESS_OK) {
      goto fail;
    }
    break;
  case PIECE_TYPE_KNIGHT:
    if ((err = knight_piece_new(&piece_promote, pawn->piece.id, pawn->piece.side, move->position_to)) != CHESS_OK) {
      goto fail;
    }
    break;
  default:
    err = CHESS_ERROR_INVALID_ARGS;
    goto fail;
  }
  if ((err = board_register_piece(board, piece_promote)) != CHESS_OK) {
    goto fail;
  }
  _pawn_free(pawn);
  return CHESS_OK;
fail:
  if (piece_promote != NULL) {
    piece_promote->piece_free(piece_promote);
    piece_promote = NULL;
  }
  return err;
}

static int _pawn_piece_clone(piece_t **pawn_piece_out, piece_t *piece) {
  if (!(pawn_piece_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn, *cloned_pawn;
  if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _pawn_clone(&cloned_pawn, pawn)) != CHESS_OK) {
    return err;
  }
  *pawn_piece_out = &cloned_pawn->piece;
  return CHESS_OK;
}

static int _pawn_piece_get_moves(move_array_t **move_array_out, piece_t *piece, board_t *board) {
  if (!(move_array_out && piece && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn;
  if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _pawn_get_moves(move_array_out, pawn, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

static int _pawn_piece_free(piece_t *piece) {
  if (!piece) {
    return CHESS_OK;
  }
  int err;
  pawn_t *pawn;
  if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _pawn_free(pawn)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int pawn_piece_new(piece_t **piece_t, piece_id_t piece_id, side_t side, vector2_t position) {
  if (!(piece_t && is_piece_id_valid(piece_id) && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn;
  if ((err = _pawn_new(&pawn, piece_id, side, position)) != CHESS_OK) {
    return err;
  }
  *piece_t = &pawn->piece;
  return CHESS_OK;
}

int pawn_piece_cast(pawn_t **pawn_out, piece_t *piece) {
  if (!(pawn_out && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (piece->type == PIECE_TYPE_PAWN) {
    *pawn_out = (pawn_t *)(piece - offsetof(pawn_t, piece));
    return CHESS_OK;
  }
  return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
}

int pawn_piece_flag_can_get_en_passant(piece_t *piece, move_t *move) {
  if (!(piece && move)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn;
  if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _pawn_flag_can_get_en_passant(pawn, move)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int pawn_piece_promote(piece_t *piece, move_t *move, board_t *board) {
  if (!(piece && move && board)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  pawn_t *pawn;
  if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
    return err;
  }
  if ((err = _pawn_promote(pawn, move, board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int board_is_position_get_attacked_by_pawn(bool *bool_out, board_t *board, side_t side, vector2_t position) {
  if (!(bool_out && board && is_side_valid(side) && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(position, direction);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    bool has_piece_on_position;
    if ((err = board_has_piece_on_position(&has_piece_on_position, board, position_to)) != CHESS_OK) {
      return err;
    }
    if (!has_piece_on_position) {
      continue;
    }
    piece_t *piece;
    if ((err = board_get_piece_by_position(&piece, board, position_to)) != CHESS_OK) {
      return err;
    }
    pawn_t *pawn;
    if ((err = pawn_piece_cast(&pawn, piece)) != CHESS_OK) {
      continue;
    }
    if (is_opposite_side(side, pawn->piece.side)) {
      *bool_out = true;
      return CHESS_OK;
    }
  }
  *bool_out = false;
  return CHESS_OK;
}