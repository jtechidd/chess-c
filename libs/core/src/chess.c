#include <assert.h>
#include <stdio.h>

#include "core/chess.h"
#include "core/common.h"
#include "core/move.h"
#include "core/piece.h"
#include "core/piece_db.h"
#include "core/pieces/bishop.h"
#include "core/pieces/king.h"
#include "core/pieces/knight.h"
#include "core/pieces/pawn.h"
#include "core/pieces/queen.h"
#include "core/pieces/rook.h"
#include "core/utils.h"
#include "core/vector2.h"

static void ch_cell_init_with_piece(ch_cell_t *cell, ch_piece_t *piece) {
  cell->piece_id = piece->id;
  cell->has_piece = true;
}

static void ch_board_place_piece(ch_board_t *board, ch_piece_t *piece) {
  ch_cell_init_with_piece(&board->table[piece->position.i][piece->position.j],
                          piece);
}

static void ch_board_clear(ch_board_t *board) {
  memset(board, 0, sizeof(ch_board_t));
}

void ch_chess_spawn_piece(ch_chess_t *chess, ch_side_t side,
                          ch_piece_type_t type, ch_vector2_t position,
                          ch_piece_data_t data,
                          const ch_piece_methods_t *methods) {
  ch_piece_t *piece;
  piece = ch_piece_db_create_piece(&chess->piece_db, side, type, position, data,
                                   methods);
  ch_board_place_piece(&chess->board, piece);
}

void ch_chess_init_standard(ch_chess_t *chess) {
  memset(chess, 0, sizeof(ch_chess_t));
  chess->turn = CH_SIDE_WHITE;
  chess->turn_count = 0;

  ch_chess_spawn_rook(chess, CH_SIDE_BLACK, ch_vector2_make(0, 0));
  ch_chess_spawn_knight(chess, CH_SIDE_BLACK, ch_vector2_make(0, 1));
  ch_chess_spawn_bishop(chess, CH_SIDE_BLACK, ch_vector2_make(0, 2));
  ch_chess_spawn_queen(chess, CH_SIDE_BLACK, ch_vector2_make(0, 3));
  ch_chess_spawn_king(chess, CH_SIDE_BLACK, ch_vector2_make(0, 4));
  ch_chess_spawn_bishop(chess, CH_SIDE_BLACK, ch_vector2_make(0, 5));
  ch_chess_spawn_knight(chess, CH_SIDE_BLACK, ch_vector2_make(0, 6));
  ch_chess_spawn_rook(chess, CH_SIDE_BLACK, ch_vector2_make(0, 7));

  for (uint8_t i = 0; i < 8; i++) {
    ch_chess_spawn_pawn(chess, CH_SIDE_BLACK, ch_vector2_make(1, i));
  }

  ch_chess_spawn_rook(chess, CH_SIDE_WHITE, ch_vector2_make(7, 0));
  ch_chess_spawn_knight(chess, CH_SIDE_WHITE, ch_vector2_make(7, 1));
  ch_chess_spawn_bishop(chess, CH_SIDE_WHITE, ch_vector2_make(7, 2));
  ch_chess_spawn_queen(chess, CH_SIDE_WHITE, ch_vector2_make(7, 3));
  ch_chess_spawn_king(chess, CH_SIDE_WHITE, ch_vector2_make(7, 4));
  ch_chess_spawn_bishop(chess, CH_SIDE_WHITE, ch_vector2_make(7, 5));
  ch_chess_spawn_knight(chess, CH_SIDE_WHITE, ch_vector2_make(7, 6));
  ch_chess_spawn_rook(chess, CH_SIDE_WHITE, ch_vector2_make(7, 7));

  for (uint8_t i = 0; i < 8; i++) {
    ch_chess_spawn_pawn(chess, CH_SIDE_WHITE, ch_vector2_make(6, i));
  }
}

static void ch_chess_update_board(ch_chess_t *chess) {
  ch_board_t *board = &chess->board;
  ch_piece_db_t *piece_db = &chess->piece_db;
  ch_board_clear(&chess->board);
  for (uint8_t i = 0; i < piece_db->total; i++) {
    ch_piece_t *piece = ch_piece_db_get_by_index(piece_db, i);
    if (!piece->is_captured) {
      ch_board_place_piece(board, piece);
    }
  }
}

ch_piece_t *ch_chess_get_piece_on_position(ch_chess_t *chess,
                                           ch_vector2_t position) {
  if (!ch_is_position_in_bound(position)) {
    return NULL;
  }
  ch_cell_t *cell = &chess->board.table[position.i][position.j];
  if (!cell->has_piece) {
    return NULL;
  }
  assert(cell->piece_id != CH_EMPTY);
  return ch_piece_db_get_by_id(&chess->piece_db, cell->piece_id);
}

static bool ch_chess_is_position_safe(ch_chess_t *chess,
                                      ch_vector2_t position) {
  if (!ch_is_position_in_bound(position))
    return false;
  if (!ch_chess_is_position_safe_from_rook(chess, position))
    return false;
  if (!ch_chess_is_position_safe_from_knight(chess, position))
    return false;
  if (!ch_chess_is_position_safe_from_bishop(chess, position))
    return false;
  if (!ch_chess_is_position_safe_from_queen(chess, position))
    return false;
  if (!ch_chess_is_position_safe_from_king(chess, position))
    return false;
  if (!ch_chess_is_position_safe_from_pawn(chess, position))
    return false;
  return true;
}

bool ch_chess_is_position_safe_to_move_to(ch_chess_t *chess,
                                          ch_vector2_t position) {
  if (ch_chess_get_piece_on_position(chess, position) != NULL) {
    return false;
  }
  return ch_chess_is_position_safe(chess, position);
}

static void ch_chess_mutate_pieces_and_board_state_unchecked(
    ch_chess_t *chess, ch_move_t move, ch_apply_move_payload_t *payload) {
  // 1. Get corresponding pieces
  ch_piece_t *piece, *taking_piece, *castling_rook;
  piece = ch_piece_db_get_by_id(&chess->piece_db, payload->piece_id);
  taking_piece =
      ch_piece_db_get_by_id(&chess->piece_db, payload->taking_piece_id);
  castling_rook =
      ch_piece_db_get_by_id(&chess->piece_db, payload->castling_rook_id);

  // 2. Mutate pieces state
  piece->position = move.position_to;
  piece->move_count++;
  piece->latest_move_turn_count = chess->turn_count;
  if (move.is_taking) {
    assert(taking_piece != NULL);
    taking_piece->is_captured = true;
  }
  if (move.promote_to != CH_EMPTY) {
    piece->type = move.promote_to;
  }
  if (castling_rook != NULL) {
    assert(piece->type == CH_PIECE_TYPE_KING);
    castling_rook->position = payload->castling_rook_position_to;
    castling_rook->move_count++;
    castling_rook->latest_move_turn_count = chess->turn_count;
  }

  // 3. Mutate board state
  ch_chess_update_board(chess);
}

static bool ch_chess_is_king_safe_after_move(ch_chess_t *chess, ch_move_t move,
                                             ch_apply_move_payload_t *payload) {
  // 1. Clone chess
  ch_chess_t chess_cpy;
  memcpy(&chess_cpy, chess, sizeof(ch_chess_t));

  // 2. Try mutate cloned chess pieces and board state
  ch_chess_mutate_pieces_and_board_state_unchecked(&chess_cpy, move, payload);

  // 3. Check whether king is safe after mutated cloned chess state
  ch_piece_t *king =
      ch_piece_db_get_king_by_side(&chess_cpy.piece_db, chess_cpy.turn);
  assert(king != NULL);
  if (!ch_chess_is_position_safe(&chess_cpy, king->position)) {
    return false;
  }
  return true;
}

ch_error_t ch_chess_apply_move(ch_chess_t *chess, ch_move_t move) {
  ch_piece_t *piece, *taking_piece;
  ch_apply_move_payload_t payload = {0};
  piece = ch_chess_get_piece_on_position(chess, move.position_from);

  // 1. Perform preliminary move validation
  if (!(ch_is_position_in_bound(move.position_from) &&
        ch_is_position_in_bound(move.position_to))) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (piece == NULL) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.piece_type != CH_EMPTY && piece->type != move.piece_type) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (!ch_vector2_equal(piece->position, move.position_from)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (piece->side != chess->turn) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.is_taking) {
    taking_piece = ch_chess_get_piece_on_position(chess, move.position_to);
    // If the piece is pawn, and taking piece is empty, may be it is en passant.
    // The pawn move validation will handle this again later.
    if (piece->type != CH_PIECE_TYPE_PAWN && !taking_piece) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    if (taking_piece && taking_piece->side == piece->side) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  } else if (ch_chess_get_piece_on_position(chess, move.position_to)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.promote_to != CH_EMPTY && piece->type != CH_PIECE_TYPE_PAWN) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  // 2. Fill some part of payload
  payload.piece_id = piece->id;
  if (taking_piece != NULL) {
    payload.taking_piece_id = taking_piece->id;
  }

  // 2. Perform specific move validation by piece type
  // This will update some part of payload. Specifically, update taking piece
  // from pawn en passant, or update castling rook from king castling.
  if (ch_piece_validate_move(piece, chess, move, &payload) != CH_ERR_SUCCESS) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  // 3. Check if king is safe after move
  if (!ch_chess_is_king_safe_after_move(chess, move, &payload)) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  // 4. Mutate pieces and board state
  ch_chess_mutate_pieces_and_board_state_unchecked(chess, move, &payload);

  // 5. Flip turn and increase number of turns
  if (chess->turn == CH_SIDE_WHITE) {
    chess->turn = CH_SIDE_BLACK;
  } else {
    chess->turn = CH_SIDE_WHITE;
  }
  chess->turn_count++;

  return CH_ERR_SUCCESS;
}

ch_error_t ch_chess_apply_move_lan(ch_chess_t *chess,
                                   const char *move_notation) {
  ch_move_t move;
  ch_error_t error;

  error = ch_move_parse_lan(&move, move_notation);
  if (error != CH_ERR_SUCCESS) {
    return error;
  }
  error = ch_chess_apply_move(chess, move);
  if (error != CH_ERR_SUCCESS) {
    return error;
  }

  return CH_ERR_SUCCESS;
}

void ch_chess_print_board(ch_chess_t *chess) {
  for (uint8_t i = 0; i < CH_BOARD_HEIGHT; i++) {
    for (uint8_t j = 0; j < CH_BOARD_WIDTH; j++) {
      ch_piece_t *piece =
          ch_chess_get_piece_on_position(chess, ch_vector2_make(i, j));
      if ((i + j) % 2 == 1) {
        printf("\033[40m");
      } else {
        printf("\033[0m");
      }
      if (piece == NULL) {
        putchar(' ');
      } else {
        if (piece->side == CH_SIDE_BLACK) {
          printf("\033[90m");
        }
        putchar(ch_piece_type_to_char(piece->type));
      }
      printf("\033[0m");
    }
    putchar('\n');
  }
}