#ifndef CHESS_H
#define CHESS_H

#include "defines.h"
#include "move_db.h"
#include "piece_db.h"

#include <stdint.h>

typedef struct chess_t chess_t;
typedef struct board_t board_t;
typedef struct cell_t cell_t;

struct cell_t {
  uint8_t has_piece;
  uint8_t piece_id;
};

void cell_init_with_piece(cell_t *cell, piece_t *piece);

struct board_t {
  cell_t table[BOARD_HEIGHT][BOARD_WIDTH];
  piece_db_t piece_db;
};

void board_update_table(board_t *board);
void board_init_standard(board_t *board);
uint8_t board_is_position_empty(board_t *board, vector2_t *position);
piece_t *board_get_piece_on_position(board_t *board, vector2_t *position);
void board_apply_move(board_t *board, move_t *move);
uint8_t board_is_king_safe(board_t *board, uint8_t side);
void board_memcpy(board_t *dest, board_t *src);

struct chess_t {
  uint8_t turn;
  board_t board;
  move_db_t move_db;
};

void chess_init_standard(chess_t *chess);
void chess_fill_moves(chess_t *chess);
move_t *chess_add_legal_move(chess_t *chess, move_t *move);
void chess_memcpy(chess_t *dest, chess_t *src);
void chess_debug(chess_t *chess);

#endif
