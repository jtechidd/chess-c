#include "board.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "enums.h"
#include "errno.h"
#include "move/move.h"
#include "move/move_array.h"
#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"
#include "utils.h"

int board_register_pieces_new_game(board_t *board);
int board_register_pieces_from_board(board_t *board_dst, board_t *board_src);
int board_update_cells(board_t *board);
int board_pieces_free(board_t *board);

int board_new(board_t **board_out) {
  if (!board_out) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  board_t *board = (board_t *)malloc(sizeof(board_t));
  if (!board) {
    return CHESS_ERROR_NO_MEMORY;
  }

  if ((err = board_register_pieces_new_game(board)) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_update_cells(board)) != CHESS_OK) {
    goto fail;
  }

  *board_out = board;
  return CHESS_OK;
fail:
  if (board != NULL) {
    board_free(board);
  }
  return err;
}

int board_clone(board_t **board_out, board_t *board_src) {
  if (!(board_out && board_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  board_t *board = (board_t *)malloc(sizeof(board_t));
  if (!board) {
    return CHESS_ERROR_NO_MEMORY;
  }
  if ((err = board_register_pieces_from_board(board, board_src)) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_update_cells(board)) != CHESS_OK) {
    goto fail;
  }

  *board_out = board;
  return CHESS_OK;
fail:
  if (board != NULL) {
    board_free(board);
    board = NULL;
  }
  return err;
}

int board_clear_cells(board_t *board) {
  if (!board) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  memset(board->cells, 0, sizeof(board->cells));
  return CHESS_OK;
}

int board_update_cells(board_t *board) {
  if (!board) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  if ((err = board_clear_cells(board)) != CHESS_OK) {
    return err;
  }
  for (piece_id_t id = 0; id < TOTAL_PIECES; id++) {
    piece_t *piece;
    if ((err = board_get_piece_by_id(&piece, board, id)) != CHESS_OK) {
      return err;
    }
    if (piece->is_captured) {
      continue;
    }
    board->cells[piece->position.i][piece->position.j] =
        cell_make(true, piece->id);
  }
  return CHESS_OK;
}

int board_register_pieces_new_game(board_t *board) {
  if (!board) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  if ((err = board_register_piece(
           board, (piece_t *)rook_new(PIECE_ID_BLACK_ROOK_1, SIDE_BLACK,
                                      vector2_make(0, 0)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)knight_new(PIECE_ID_BLACK_KNIGHT_1, SIDE_BLACK,
                                        vector2_make(0, 1)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)bishop_new(PIECE_ID_BLACK_BISHOP_1, SIDE_BLACK,
                                        vector2_make(0, 2)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)queen_new(PIECE_ID_BLACK_QUEEN, SIDE_BLACK,
                                       vector2_make(0, 3)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)king_new(PIECE_ID_BLACK_KING, SIDE_BLACK,
                                      vector2_make(0, 4)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)bishop_new(PIECE_ID_BLACK_BISHOP_2, SIDE_BLACK,
                                        vector2_make(0, 5)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)knight_new(PIECE_ID_BLACK_KNIGHT_2, SIDE_BLACK,
                                        vector2_make(0, 6)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)rook_new(PIECE_ID_BLACK_ROOK_2, SIDE_BLACK,
                                      vector2_make(0, 7)))) != CHESS_OK) {
    goto fail;
  }
  for (piece_id_t j = 0; j < 8; j++) {
    if ((err = board_register_piece(
             board, (piece_t *)pawn_new(PIECE_ID_BLACK_PAWN_1 + j, SIDE_BLACK,
                                        vector2_make(1, j)))) != CHESS_OK) {
      goto fail;
    }
  }

  if ((err = board_register_piece(
           board, (piece_t *)rook_new(PIECE_ID_WHITE_ROOK_1, SIDE_WHITE,
                                      vector2_make(7, 0)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)knight_new(PIECE_ID_WHITE_KNIGHT_1, SIDE_WHITE,
                                        vector2_make(7, 1)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)bishop_new(PIECE_ID_WHITE_BISHOP_1, SIDE_WHITE,
                                        vector2_make(7, 2)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)queen_new(PIECE_ID_WHITE_QUEEN, SIDE_WHITE,
                                       vector2_make(7, 3)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)king_new(PIECE_ID_WHITE_KING, SIDE_WHITE,
                                      vector2_make(7, 4)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)bishop_new(PIECE_ID_WHITE_BISHOP_2, SIDE_WHITE,
                                        vector2_make(7, 5)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)knight_new(PIECE_ID_WHITE_KNIGHT_2, SIDE_WHITE,
                                        vector2_make(7, 6)))) != CHESS_OK) {
    goto fail;
  }
  if ((err = board_register_piece(
           board, (piece_t *)rook_new(PIECE_ID_WHITE_ROOK_2, SIDE_WHITE,
                                      vector2_make(7, 7)))) != CHESS_OK) {
    goto fail;
  }
  for (piece_id_t j = 0; j < 8; j++) {
    if ((err = board_register_piece(
             board, (piece_t *)pawn_new(PIECE_ID_WHITE_PAWN_1 + j, SIDE_WHITE,
                                        vector2_make(6, j)))) != CHESS_OK) {
      goto fail;
    }
  }
  return CHESS_OK;
fail:
  board_pieces_free(board);
  return err;
}

int board_register_pieces_from_board(board_t *board_dst, board_t *board_src) {
  if (!(board_dst && board_src)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_t *piece, *new_piece;
  for (piece_id_t piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
    if ((err = board_get_piece_by_id(&piece, board_src, piece_id)) !=
        CHESS_OK) {
      goto fail;
    }
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
    default:
      err = CHESS_ERROR_INVALID_STATE;
      goto fail;
    }
    if ((err = board_register_piece(board_dst, new_piece)) != CHESS_OK) {
      goto fail;
    }
  }
  return CHESS_OK;
fail:
  if (new_piece != NULL) {
    new_piece->piece_free(new_piece);
    new_piece = NULL;
  }
  return err;
}

int board_register_piece(board_t *board, piece_t *piece) {
  if (!(board && piece)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  board->pieces[piece->id] = piece;
  return CHESS_OK;
}

int board_get_piece_by_id(piece_t **piece_out, board_t *board,
                          piece_id_t piece_id) {
  if (!(piece_out && board && is_piece_id_valid(piece_id))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  *piece_out = board->pieces[piece_id];
  return CHESS_OK;
}

int board_get_piece_by_position(piece_t **piece_out, board_t *board,
                                vector2_t position) {
  if (!(piece_out && board && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  cell_t cell = board->cells[position.i][position.j];
  if (!cell.has_piece) {
    *piece_out = NULL;
    return CHESS_OK;
  }
  piece_id_t piece_id = cell.piece_id;
  if (!is_piece_id_valid(piece_id)) {
    return CHESS_ERROR_INVALID_STATE;
  }
  *piece_out = board->pieces[piece_id];
  return CHESS_OK;
}

int board_has_piece_on_position(bool *bool_out, board_t *board,
                                vector2_t position) {
  if (!(bool_out && board && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_t *piece;
  if ((err = board_get_piece_by_position(&piece, board, position)) !=
      CHESS_OK) {
    return err;
  }
  *bool_out = piece != NULL;
  return CHESS_OK;
}

int board_can_take_position(bool *bool_out, board_t *board, piece_t *piece,
                            vector2_t position) {
  if (!(bool_out && board && piece && is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_t *piece_on_position;
  if ((err = board_get_piece_by_position(&piece_on_position, board,
                                         position)) != CHESS_OK) {
    return err;
  }
  *bool_out = piece_on_position && piece_is_opposite(piece, piece_on_position);
  return CHESS_OK;
}

int board_get_moves(move_array_t **move_array_out, board_t *board,
                    side_t side) {
  if (!(move_array_out && board && is_side_valid(side))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_id_t piece_id_start, piece_id_end;
  if (side == SIDE_WHITE) {
    piece_id_start = PIECE_ID_WHITE_ROOK_1;
    piece_id_end = PIECE_ID_WHITE_PAWN_8;
  } else if (side == SIDE_BLACK) {
    piece_id_start = PIECE_ID_BLACK_ROOK_1;
    piece_id_end = PIECE_ID_BLACK_PAWN_8;
  }
  move_array_t *all_moves = move_array_new();
  move_array_t *piece_moves;
  board_t *cloned_board;
  for (piece_id_t piece_id = piece_id_start; piece_id <= piece_id_end;
       piece_id++) {
    piece_t *piece;
    if ((err = board_get_piece_by_id(&piece, board, piece_id)) != CHESS_OK) {
      goto fail;
    }
    if (piece->is_captured) {
      continue;
    }
    piece_moves = piece->piece_get_moves(piece, board);
    for (size_t i = 0; i < piece_moves->length; i++) {
      move_t *move = move_array_get_index(piece_moves, i);
      if ((err = board_clone(&cloned_board, board)) != CHESS_OK) {
        goto fail;
      }
      if ((err = board_apply_move(cloned_board, move)) != CHESS_OK) {
        goto fail;
      }
      bool is_king_get_attacked;
      if ((err = board_is_king_get_attacked(&is_king_get_attacked, cloned_board,
                                            side)) != CHESS_OK) {
        goto fail;
      }
      if (!is_king_get_attacked) {
        move_array_add(all_moves, move);
        move_array_set_index(piece_moves, i, NULL);
      }
      board_free(cloned_board);
    }

    move_array_free(piece_moves);
  }

  *move_array_out = all_moves;
  return CHESS_OK;
fail:
  if (all_moves != NULL) {
    move_array_free(all_moves);
    all_moves = NULL;
  }
  if (piece_moves != NULL) {
    move_array_free(piece_moves);
    piece_moves = NULL;
  }
  if (cloned_board != NULL) {
    board_free(cloned_board);
    cloned_board = NULL;
  }
  return err;
}

int board_apply_move(board_t *board, move_t *move) {
  if (!(board && move)) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_id_t piece_id = move->piece_id;
  piece_t *piece, *take_piece;
  if ((err = board_get_piece_by_id(&piece, board, piece_id)) != CHESS_OK) {
    return err;
  }
  if (move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) {
    pawn_flag_can_get_en_passant(piece, move);

    piece->position = move->position_to;
    piece->moving_count++;
  }
  if (move->flags & MOVE_FLAGS_HAS_TAKING_PIECE) {
    piece_id_t take_piece_id = move->take_piece_id;
    if ((err = board_get_piece_by_id(&take_piece, board, take_piece_id)) !=
        CHESS_OK) {
      return err;
    }

    take_piece->is_captured = 1;
  }
  if (move->flags & MOVE_FLAGS_HAS_PROMOTION) {
    pawn_promote(piece, move, board);
  }
  if (move->flags & MOVE_FLAGS_HAS_CASTLING) {
    king_castle(piece, move, board);
  }
  if ((err = board_update_cells(board)) != CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int board_is_position_get_attacked(bool *bool_out, board_t *board, side_t side,
                                   vector2_t position) {
  board_is_position_being_attacked_by_piece_fn *check_fns[] = {
      board_is_position_being_attacked_by_pawn,
      board_is_position_being_attacked_by_rook,
      board_is_position_being_attacked_by_knight,
      board_is_position_being_attacked_by_bishop,
      board_is_position_being_attacked_by_queen,
      board_is_position_being_attacked_by_king,
  };
  for (size_t i = 0; i < 6; i++) {
    board_is_position_being_attacked_by_piece_fn *check_fn = check_fns[i];
    if (check_fn(board, side, position)) {
      *bool_out = true;
      return CHESS_OK;
    }
  }
  bool_out = false;
  return CHESS_OK;
}

int board_is_position_safe_to_move_to(bool *bool_out, board_t *board,
                                      side_t side, vector2_t position) {
  if (!(bool_out && board && is_side_valid(side) &&
        is_position_in_bound(position))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  bool has_piece_on_position;
  if ((err = board_has_piece_on_position(&has_piece_on_position, board,
                                         position)) != CHESS_OK) {
    return err;
  }
  bool is_position_get_attacked;
  if ((err = board_is_position_get_attacked(&is_position_get_attacked, board,
                                            side, position)) != CHESS_OK) {
    return err;
  }
  *bool_out = !has_piece_on_position && !is_position_get_attacked;
  return CHESS_OK;
}

int board_is_king_get_attacked(bool *bool_out, board_t *board, side_t side) {
  if (!(bool_out && board && is_side_valid(side))) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  piece_id_t king_piece_id;
  if (side == SIDE_WHITE) {
    king_piece_id = PIECE_ID_WHITE_KING;
  } else if (side == SIDE_BLACK) {
    king_piece_id = PIECE_ID_BLACK_KING;
  }
  piece_t *king_piece;
  if ((err = board_get_piece_by_id(&king_piece, board, king_piece_id)) !=
      CHESS_OK) {
    return err;
  }
  king_t *king;
  if (!(king = king_cast(king_piece))) {
    return CHESS_ERROR_CAST_PIECE_TYPE_MISMATCH;
  }
  if ((err = board_is_position_get_attacked(bool_out, board, king->piece.side,
                                            king->piece.position)) !=
      CHESS_OK) {
    return err;
  }
  return CHESS_OK;
}

int board_pieces_free(board_t *board) {
  if (!board) {
    return CHESS_OK;
  }
  int err;
  for (piece_id_t piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
    if (!board->pieces[piece_id]) {
      continue;
    }
    piece_t *piece;
    if ((err = board_get_piece_by_id(&piece, board, piece_id)) != CHESS_OK) {
      return err;
    }
    piece->piece_free(piece);
    board->pieces[piece_id] = NULL;
  }
  return CHESS_OK;
}

int board_free(board_t *board) {
  if (!board) {
    return CHESS_OK;
  }
  int err;
  if ((err = board_pieces_free(board)) != CHESS_OK) {
    return err;
  }
  free(board);
  return CHESS_OK;
}

int board_debug(board_t *board) {
  int err;
  for (size_t i = 0; i < BOARD_HEIGHT; i++) {
    for (size_t j = 0; j < BOARD_WIDTH; j++) {
      piece_t *piece;
      if ((err = board_get_piece_by_position(&piece, board,
                                             vector2_make(i, j))) != CHESS_OK) {
        return err;
      }
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
      default:
        return CHESS_ERROR_INVALID_STATE;
      }
    }
    printf("\n");
  }
  return CHESS_OK;
}