#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enums.h"
#include "move/move_array.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"
#include "utils.h"

void board_register_pieces_new_game(board_t *board);
void board_register_pieces_from_board(board_t *board_dst, board_t *board_src);
void board_update_cells(board_t *board);
void board_apply_move(board_t *board, move_t *move);

board_t *board_new() {
  board_t *board = (board_t *)malloc(sizeof(board_t));

  board_register_pieces_new_game(board);
  board_update_cells(board);

  return board;
}

board_t *board_clone(board_t *board_src) {
  board_t *b = (board_t *)malloc(sizeof(board_t));

  board_register_pieces_from_board(b, board_src);
  board_update_cells(b);

  return b;
}

void board_clear_cells(board_t *b) { memset(b->cells, 0, sizeof(b->cells)); }

void board_update_cells(board_t *board) {
  board_clear_cells(board);
  for (piece_id_t id = 0; id < TOTAL_PIECES; id++) {
    piece_t *piece = board_get_piece_by_id(board, id);
    if (piece->is_captured) {
      continue;
    }
    board->cells[piece->position.i][piece->position.j] =
        cell_make(1, piece->id);
  }
}

void board_register_pieces_new_game(board_t *board) {
  board_register_piece(board,
                       (piece_t *)rook_new(PIECE_ID_BLACK_ROOK_1, SIDE_BLACK,
                                           vector2_make(0, 0)));
  board_register_piece(board,
                       (piece_t *)knight_new(PIECE_ID_BLACK_KNIGHT_1,
                                             SIDE_BLACK, vector2_make(0, 1)));
  board_register_piece(board,
                       (piece_t *)bishop_new(PIECE_ID_BLACK_BISHOP_1,
                                             SIDE_BLACK, vector2_make(0, 2)));
  board_register_piece(board,
                       (piece_t *)queen_new(PIECE_ID_BLACK_QUEEN, SIDE_BLACK,
                                            vector2_make(0, 3)));
  board_register_piece(
      board,
      (piece_t *)king_new(PIECE_ID_BLACK_KING, SIDE_BLACK, vector2_make(0, 4)));
  board_register_piece(board,
                       (piece_t *)bishop_new(PIECE_ID_BLACK_BISHOP_2,
                                             SIDE_BLACK, vector2_make(0, 5)));
  board_register_piece(board,
                       (piece_t *)knight_new(PIECE_ID_BLACK_KNIGHT_2,
                                             SIDE_BLACK, vector2_make(0, 6)));
  board_register_piece(board,
                       (piece_t *)rook_new(PIECE_ID_BLACK_ROOK_2, SIDE_BLACK,
                                           vector2_make(0, 7)));
  for (piece_id_t j = 0; j < 8; j++) {
    board_register_piece(board,
                         (piece_t *)pawn_new(PIECE_ID_BLACK_PAWN_1 + j,
                                             SIDE_BLACK, vector2_make(1, j)));
  }

  board_register_piece(board,
                       (piece_t *)rook_new(PIECE_ID_WHITE_ROOK_1, SIDE_WHITE,
                                           vector2_make(7, 0)));
  board_register_piece(board,
                       (piece_t *)knight_new(PIECE_ID_WHITE_KNIGHT_1,
                                             SIDE_WHITE, vector2_make(7, 1)));
  board_register_piece(board,
                       (piece_t *)bishop_new(PIECE_ID_WHITE_BISHOP_1,
                                             SIDE_WHITE, vector2_make(7, 2)));
  board_register_piece(board,
                       (piece_t *)queen_new(PIECE_ID_WHITE_QUEEN, SIDE_WHITE,
                                            vector2_make(7, 3)));
  board_register_piece(
      board,
      (piece_t *)king_new(PIECE_ID_WHITE_KING, SIDE_WHITE, vector2_make(7, 4)));
  board_register_piece(board,
                       (piece_t *)bishop_new(PIECE_ID_WHITE_BISHOP_2,
                                             SIDE_WHITE, vector2_make(7, 5)));
  board_register_piece(board,
                       (piece_t *)knight_new(PIECE_ID_WHITE_KNIGHT_2,
                                             SIDE_WHITE, vector2_make(7, 6)));
  board_register_piece(board,
                       (piece_t *)rook_new(PIECE_ID_WHITE_ROOK_2, SIDE_WHITE,
                                           vector2_make(7, 7)));
  for (piece_id_t j = 0; j < 8; j++) {
    board_register_piece(board,
                         (piece_t *)pawn_new(PIECE_ID_WHITE_PAWN_1 + j,
                                             SIDE_WHITE, vector2_make(6, j)));
  }
}

void board_register_pieces_from_board(board_t *board_dst, board_t *board_src) {
  for (piece_id_t piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
    piece_t *piece = board_get_piece_by_id(board_src, piece_id);
    piece_t *new_piece;
    switch (piece->type) {
    case PIECE_TYPE_ROOK:
      new_piece = (piece_t *)rook_clone((rook_t *)piece);
      break;
    case PIECE_TYPE_KNIGHT:
      new_piece = (piece_t *)knight_clone((knight_t *)piece);
      break;
    case PIECE_TYPE_BISHOP:
      new_piece = (piece_t *)bishop_clone((bishop_t *)piece);
      break;
    case PIECE_TYPE_QUEEN:
      new_piece = (piece_t *)queen_clone((queen_t *)piece);
      break;
    case PIECE_TYPE_KING:
      new_piece = (piece_t *)king_clone((king_t *)piece);
      break;
    case PIECE_TYPE_PAWN:
      new_piece = (piece_t *)pawn_clone((pawn_t *)piece);
      break;
    }
    board_register_piece(board_dst, new_piece);
  }
}

void board_register_piece(board_t *board, piece_t *piece) {
  board->pieces[piece->id] = piece;
}

piece_t *board_get_piece_by_id(board_t *board, piece_id_t piece_id) {
  if (!is_piece_id_valid(piece_id)) {
    return NULL;
  }
  return board->pieces[piece_id];
}

piece_t *board_get_piece_by_position(board_t *board, vector2_t position) {
  if (!is_position_in_bound(position)) {
    return NULL;
  }
  cell_t cell = board->cells[position.i][position.j];
  if (!cell.has_piece) {
    return NULL;
  }
  piece_id_t piece_id = cell.piece_id;
  if (!is_piece_id_valid(piece_id)) {
    return NULL;
  }
  return board->pieces[piece_id];
}

uint8_t board_has_piece_on_position(board_t *board, vector2_t position) {
  return board_get_piece_by_position(board, position) != NULL;
}

uint8_t board_can_take_position(board_t *board, piece_t *piece,
                                vector2_t position) {
  piece_t *piece_on_position = board_get_piece_by_position(board, position);
  return piece_on_position && piece_is_opposite(piece, piece_on_position);
}

move_array_t *board_get_moves(board_t *board, side_t side) {
  piece_id_t piece_id_start, piece_id_end;
  if (side == SIDE_WHITE) {
    piece_id_start = PIECE_ID_WHITE_ROOK_1;
    piece_id_end = PIECE_ID_WHITE_PAWN_8;
  } else if (side == SIDE_BLACK) {
    piece_id_start = PIECE_ID_BLACK_ROOK_1;
    piece_id_end = PIECE_ID_BLACK_PAWN_8;
  }

  move_array_t *all_moves = move_array_new();
  for (piece_id_t pieceid = piece_id_start; pieceid <= piece_id_end;
       pieceid++) {
    piece_t *piece = board_get_piece_by_id(board, pieceid);
    if (piece->is_captured) {
      continue;
    }
    move_array_t *piece_moves = piece->piece_get_moves(piece, board);
    for (size_t i = 0; i < piece_moves->length; i++) {
      move_t *move = move_array_get_index(piece_moves, i);
      board_t *cloned_board = board_clone(board);
      board_apply_move(cloned_board, move);
      if (board_is_king_get_attacked(cloned_board, side)) {
        move_free(move);
      } else {
        move_array_add(all_moves, move);
      }
      board_free(cloned_board);
    }

    move_array_shallow_free(piece_moves);
  }

  return all_moves;
}

void board_apply_move(board_t *board, move_t *move) {
  piece_id_t piece_id = move->piece_id;
  piece_t *piece = board_get_piece_by_id(board, piece_id);
  if (move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) {
    pawn_flag_can_get_en_passant(piece, move);

    piece->position = move->position_to;
    piece->moving_count++;
  }
  if (move->flags & MOVE_FLAGS_HAS_TAKING_PIECE) {
    piece_id_t take_piece_id = move->take_piece_id;
    piece_t *take_piece = board_get_piece_by_id(board, take_piece_id);

    take_piece->is_captured = 1;
  }
  if (move->flags & MOVE_FLAGS_HAS_PROMOTION) {
    pawn_promote(piece, move, board);
  }
  if (move->flags & MOVE_FLAGS_HAS_CASTLING) {
    king_castle(piece, move, board);
  }
  board_update_cells(board);
}

uint8_t board_is_position_get_attacked(board_t *board, side_t side,
                                       vector2_t position) {
  board_is_position_being_attacked_by_piece_t *check_fns[] = {
      board_is_position_being_attacked_by_pawn,
      board_is_position_being_attacked_by_rook,
      board_is_position_being_attacked_by_knight,
      board_is_position_being_attacked_by_bishop,
      board_is_position_being_attacked_by_queen,
      board_is_position_being_attacked_by_king,
  };
  for (size_t i = 0; i < 6; i++) {
    board_is_position_being_attacked_by_piece_t *check_fn = check_fns[i];
    if (check_fn(board, side, position)) {
      return 1;
    }
  }
  return 0;
}

uint8_t board_is_position_safe_to_move_to(board_t *board, side_t side,
                                          vector2_t position) {
  return !board_has_piece_on_position(board, position) &&
         !board_is_position_get_attacked(board, side, position);
}

uint8_t board_is_king_get_attacked(board_t *board, side_t side) {
  piece_id_t king_piece_id;
  if (side == SIDE_WHITE) {
    king_piece_id = PIECE_ID_WHITE_KING;
  } else if (side == SIDE_BLACK) {
    king_piece_id = PIECE_ID_BLACK_KING;
  }
  piece_t *king_piece = board_get_piece_by_id(board, king_piece_id);
  king_t *king;
  if (!(king = king_cast(king_piece))) {
    return 0;
  }
  return board_is_position_get_attacked(board, king->piece.side,
                                        king->piece.position);
}

void board_free(board_t *board) {
  for (piece_id_t piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
    piece_t *piece = board_get_piece_by_id(board, piece_id);
    piece->piece_free(piece);
  }
  free(board);
}

void board_debug(board_t *board) {
  for (size_t i = 0; i < BOARD_HEIGHT; i++) {
    for (size_t j = 0; j < BOARD_WIDTH; j++) {
      piece_t *piece = board_get_piece_by_position(board, vector2_make(i, j));
      if (!piece) {
        printf(".");
        continue;
      }
      switch (piece->type) {
      case PIECE_TYPE_ROOK:
        printf("r");
        break;
      case PIECE_TYPE_KNIGHT:
        printf("k");
        break;
      case PIECE_TYPE_BISHOP:
        printf("b");
        break;
      case PIECE_TYPE_QUEEN:
        printf("q");
        break;
      case PIECE_TYPE_KING:
        printf("K");
        break;
      case PIECE_TYPE_PAWN:
        printf("p");
        break;
      }
    }
    printf("\n");
  }
}