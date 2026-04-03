#include "chess.h"
#include "defines.h"
#include "piece_db.h"

#include <stdio.h>
#include <string.h>

void cell_init_with_piece(cell_t *cell, piece_t *piece) {
  cell->has_piece = 1;
  cell->piece_id = piece->id;
}

void board_update_table(board_t *board) {
  memset(board->table, 0, sizeof(board->table));

  piece_array_t *pieces = &board->piece_db.pieces;

  for (uint8_t i = 0; i < pieces->length; i++) {
    piece_t *piece = piece_array_get_index(pieces, i);
    cell_t *cell = &board->table[piece->position.i][piece->position.j];
    if (!piece->is_captured) {
      cell_init_with_piece(cell, piece);
    }
  }
}

void board_init_standard(board_t *board) {
  memset(board, 0, sizeof(board_t));

  piece_db_t *piece_db = &board->piece_db;

  vector2_t position;
  piece_data_t piece_data;
  piece_t piece;

#define CHESS_SETUP_PIECE(type, side, i, j)                                    \
  vector2_init(&position, i, j);                                               \
  piece_data_init_empty(&piece_data);                                          \
  piece_init(&piece, type, side, &position, &piece_data);                      \
  piece_db_create(piece_db, &piece);

#define CHESS_SETUP_PIECE_ROOK(rook_type, side, i, j)                          \
  vector2_init(&position, i, j);                                               \
  piece_data_init_rook(&piece_data, rook_type);                                \
  piece_init(&piece, PIECE_TYPE_ROOK, side, &position, &piece_data);           \
  piece_db_create(piece_db, &piece);

  CHESS_SETUP_PIECE_ROOK(ROOK_TYPE_QUEEN_SIDE, SIDE_BLACK, 0, 0)
  CHESS_SETUP_PIECE(PIECE_TYPE_KNIGHT, SIDE_BLACK, 0, 1)
  CHESS_SETUP_PIECE(PIECE_TYPE_BISHOP, SIDE_BLACK, 0, 2)
  CHESS_SETUP_PIECE(PIECE_TYPE_QUEEN, SIDE_BLACK, 0, 3)
  CHESS_SETUP_PIECE(PIECE_TYPE_KING, SIDE_BLACK, 0, 4)
  CHESS_SETUP_PIECE(PIECE_TYPE_BISHOP, SIDE_BLACK, 0, 5)
  CHESS_SETUP_PIECE(PIECE_TYPE_KNIGHT, SIDE_BLACK, 0, 6)
  CHESS_SETUP_PIECE_ROOK(ROOK_TYPE_KING_SIDE, SIDE_BLACK, 0, 7)
  for (uint8_t j = 0; j < 8; j++) {
    CHESS_SETUP_PIECE(PIECE_TYPE_PAWN, SIDE_BLACK, 1, j)
  }

  CHESS_SETUP_PIECE_ROOK(ROOK_TYPE_QUEEN_SIDE, SIDE_WHITE, 7, 0)
  CHESS_SETUP_PIECE(PIECE_TYPE_KNIGHT, SIDE_WHITE, 7, 1)
  CHESS_SETUP_PIECE(PIECE_TYPE_BISHOP, SIDE_WHITE, 7, 2)
  CHESS_SETUP_PIECE(PIECE_TYPE_QUEEN, SIDE_WHITE, 7, 3)
  CHESS_SETUP_PIECE(PIECE_TYPE_KING, SIDE_WHITE, 7, 4)
  CHESS_SETUP_PIECE(PIECE_TYPE_BISHOP, SIDE_WHITE, 7, 5)
  CHESS_SETUP_PIECE(PIECE_TYPE_KNIGHT, SIDE_WHITE, 7, 6)
  CHESS_SETUP_PIECE_ROOK(ROOK_TYPE_KING_SIDE, SIDE_WHITE, 7, 7)
  for (uint8_t j = 0; j < 8; j++) {
    CHESS_SETUP_PIECE(PIECE_TYPE_PAWN, SIDE_WHITE, 6, j)
  }

#undef CHESS_SETUP_PIECE_ROOK
#undef CHESS_SETUP_PIECE

  piece_db_compute_index_standard(&board->piece_db);
  board_update_table(board);
}

void board_apply_move(board_t *board, move_t *move) {}

uint8_t board_is_king_safe(board_t *board, uint8_t side) { return 1; }

uint8_t board_is_position_empty(board_t *board, vector2_t *position) {
  return !board->table[position->i][position->j].has_piece;
}

piece_t *board_get_piece_on_position(board_t *board, vector2_t *position) {
  cell_t *cell = &board->table[position->i][position->j];
  if (!cell->has_piece) {
    return NULL;
  }
  return piece_db_get_by_id(&board->piece_db, cell->piece_id);
}

void board_memcpy(board_t *dest, board_t *src) {
  memcpy(dest, src, sizeof(board_t));
}

void chess_init_standard(chess_t *chess) {
  memset(chess, 0, sizeof(chess_t));
  board_init_standard(&chess->board);
  chess->turn = SIDE_WHITE;
}

void chess_fill_moves(chess_t *chess) {
  move_db_t *move_db = &chess->move_db;
  piece_db_t *piece_db = &chess->board.piece_db;

  move_db_init(move_db);

  piece_id_array_t *piece_ids;
  if (chess->turn == SIDE_WHITE) {
    piece_ids = &piece_db->piece_ids_white;
  } else {
    piece_ids = &piece_db->piece_ids_black;
  }

  for (uint8_t i = 0; i < piece_ids->length; i++) {
    uint8_t piece_id = piece_id_array_get_index(piece_ids, i);
    piece_t *piece = piece_db_get_by_id(piece_db, piece_id);
    piece_fill_moves(chess, piece);
  }

  move_db_compute_index(move_db);
}

move_t *chess_add_legal_move(chess_t *chess, move_t *move) {
  board_t board_snapshot;
  board_memcpy(&board_snapshot, &chess->board);
  board_apply_move(&board_snapshot, move);
  if (board_is_king_safe(&board_snapshot, chess->turn)) {
    return move_db_create(&chess->move_db, move);
  }
  return NULL;
}

void chess_debug(chess_t *chess) {
  printf("Turn: %d\n", chess->turn);
  cell_t *cell;
  for (uint8_t i = 0; i < BOARD_HEIGHT; i++) {
    for (uint8_t j = 0; j < BOARD_WIDTH; j++) {
      cell = &chess->board.table[i][j];
      printf("%d,%d\t", cell->has_piece, cell->piece_id);
    }
    printf("\n");
  }
  for (uint8_t i = 0; i < chess->board.piece_db.pieces.length; i++) {
    piece_t *piece = piece_array_get_index(&chess->board.piece_db.pieces, i);
    printf("%d: %d, %d,%d\n", piece->id, piece->type, piece->position.i,
           piece->position.j);
  }
  for (uint8_t i = 0; i < chess->board.piece_db.piece_ids_black.length; i++) {
    printf("%d ", chess->board.piece_db.piece_ids_black.array[i]);
  }
  printf("\n");
  printf("%d\n", chess->board.piece_db.piece_id_king_black);
  printf("%d\n", chess->board.piece_db.piece_id_rook_black_king_side);
  printf("%d\n", chess->board.piece_db.piece_id_rook_black_queen_side);
  for (uint8_t i = 0; i < chess->board.piece_db.piece_ids_white.length; i++) {
    printf("%d ", chess->board.piece_db.piece_ids_white.array[i]);
  }
  printf("\n");
  printf("%d\n", chess->board.piece_db.piece_id_king_white);
  printf("%d\n", chess->board.piece_db.piece_id_rook_white_king_side);
  printf("%d\n", chess->board.piece_db.piece_id_rook_white_queen_side);
}