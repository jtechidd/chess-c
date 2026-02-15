
#include <assert.h>
#include <stddef.h>

#include "../board.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "bishop.h"
#include "knight.h"
#include "pawn.h"
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
static piece_get_wchar_fn _pawn_piece_get_wchar;
pawn_t *pawn_piece_cast(piece_t *);

static pawn_t *_pawn_new(piece_id_t piece_id, side_t side, vector2_t position) {
  pawn_t *pawn = (pawn_t *)xmalloc(sizeof(pawn_t));

  pawn->piece.id = piece_id;
  pawn->piece.side = side;
  pawn->piece.type = PIECE_TYPE_PAWN;
  pawn->piece.position = position;
  pawn->piece.is_captured = false;
  pawn->piece.moving_count = 0;

  pawn->piece.piece_clone = _pawn_piece_clone;
  pawn->piece.piece_get_moves = _pawn_piece_get_moves;
  pawn->piece.piece_free = _pawn_piece_free;
  pawn->piece.piece_get_wchar = _pawn_piece_get_wchar;

  pawn->can_get_en_passant = false;

  return pawn;
}

static piece_t *_pawn_piece_clone(piece_t *piece_src) {
  pawn_t *pawn_src = pawn_piece_cast(piece_src);
  pawn_t *pawn = _pawn_new(pawn_src->piece.id, pawn_src->piece.side,
                           pawn_src->piece.position);

  pawn->piece.is_captured = pawn_src->piece.is_captured;
  pawn->piece.moving_count = pawn_src->piece.moving_count;

  pawn->can_get_en_passant = pawn_src->can_get_en_passant;

  return &pawn->piece;
}

static void pawn_add_promotion_moves(move_array_t *move_array, move_t *move) {
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

static void _pawn_add_move(pawn_t *pawn, move_array_t *move_array,
                           move_t *move) {
  side_t side = pawn->piece.side;

  bool can_be_promoted = false;
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

static void _pawn_add_moves_up(pawn_t *pawn, board_t *board,
                               move_array_t *move_array) {
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
    _pawn_add_move(pawn, move_array, move);
  }
}

static void _pawn_add_moves_take(pawn_t *pawn, board_t *board,
                                 move_array_t *move_array) {
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
    move_t *move =
        move_new_taking_piece(pawn->piece.id, position_to, take_piece->id);
    _pawn_add_move(pawn, move_array, move);
  }
}

static void _pawn_add_moves_en_passant(pawn_t *pawn, board_t *board,
                                       move_array_t *move_array) {
  side_t side = pawn->piece.side;
  vector2_t position = pawn->piece.position;

  for (size_t k = 0; k < PAWN_TAKE_TOTAL_DIRECTIONS; k++) {
    vector2_t direction = PAWN_TAKE_DIRECTIONS[k];
    vector2_t direction_side_pawn = vector2_make(0, direction.j);
    if (side == SIDE_BLACK) {
      direction = vector2_vflip(direction);
    }
    vector2_t position_to = vector2_add2(pawn->piece.position, direction);
    vector2_t position_side_pawn =
        vector2_add2(pawn->piece.position, direction_side_pawn);
    if (!is_position_in_bound(position_to)) {
      continue;
    }
    if (!is_position_in_bound(position_side_pawn)) {
      continue;
    }
    piece_t *piece = board_get_piece_by_position(board, position_side_pawn);
    pawn_t *side_pawn;
    if (!(side_pawn = pawn_piece_cast(piece))) {
      return;
    }
    if (!piece_is_opposite(&pawn->piece, piece)) {
      return;
    }
    if (!side_pawn->can_get_en_passant) {
      return;
    }
    move_t *move =
        move_new_taking_piece(pawn->piece.id, position_to, side_pawn->piece.id);
    _pawn_add_move(pawn, move_array, move);
  }
}

static move_array_t *_pawn_piece_get_moves(piece_t *piece, board_t *board) {
  pawn_t *pawn = pawn_piece_cast(piece);
  move_array_t *move_array = move_array_new();

  _pawn_add_moves_up(pawn, board, move_array);
  _pawn_add_moves_take(pawn, board, move_array);
  _pawn_add_moves_en_passant(pawn, board, move_array);

  return move_array;
}

static void _pawn_piece_free(piece_t *piece) {
  pawn_t *pawn = pawn_piece_cast(piece);
  xfree(pawn);
}

static wchar_t _pawn_piece_get_wchar(piece_t *piece) {
  pawn_t *pawn = pawn_piece_cast(piece);
  if (pawn->piece.side == SIDE_WHITE) {
    return 0x2659;
  } else if (pawn->piece.side == SIDE_BLACK) {
    return 0x265F;
  }
  return '\0';
}

piece_t *pawn_piece_new(piece_id_t piece_id, side_t side, vector2_t position) {
  pawn_t *pawn = _pawn_new(piece_id, side, position);
  return &pawn->piece;
}

pawn_t *pawn_piece_cast(piece_t *piece) {
  if (piece->type != PIECE_TYPE_PAWN) {
    return NULL;
  }
  return (pawn_t *)(piece - offsetof(pawn_t, piece));
}

void pawn_piece_flag_can_get_en_passant(piece_t *piece, move_t *move) {
  pawn_t *pawn = pawn_piece_cast(piece);
  assert(pawn != NULL);

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

void pawn_piece_promote(piece_t *piece, move_t *move, board_t *board) {
  pawn_t *pawn = pawn_piece_cast(piece);
  assert(pawn != NULL);

  switch (move->promote_to) {
  case PIECE_TYPE_QUEEN:
    board_register_piece(
        board, queen_piece_new(pawn->piece.id, piece->side, move->position_to));
    break;
  case PIECE_TYPE_ROOK:
    board_register_piece(
        board, rook_piece_new(pawn->piece.id, piece->side, move->position_to));
    break;
  case PIECE_TYPE_BISHOP:
    board_register_piece(board, bishop_piece_new(pawn->piece.id, piece->side,
                                                 move->position_to));
    break;
  case PIECE_TYPE_KNIGHT:
    board_register_piece(board, knight_piece_new(pawn->piece.id, piece->side,
                                                 move->position_to));
    break;
  default:
    break;
  }
  piece->piece_free(piece);
}

bool board_is_position_being_attacked_by_pawn(board_t *board, side_t side,
                                              vector2_t position) {
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
    if (!(pawn = pawn_piece_cast(piece))) {
      continue;
    }
    if (is_opposite_side(side, pawn->piece.side)) {
      return 1;
    }
  }
  return 0;
}