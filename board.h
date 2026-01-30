#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "cell.h"
#include "enums.h"
#include "vector2.h"

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8

typedef struct piece_t piece_t;
typedef struct move_t move_t;
typedef struct move_array_t move_array_t;

typedef struct board_t {
  cell_t cells[BOARD_HEIGHT][BOARD_WIDTH];
  piece_t *pieces[TOTAL_PIECES];
} board_t;

board_t *board_new();
board_t *board_clone(board_t *);
void board_clear_cells(board_t *);
void board_update_cells(board_t *);
void board_register_pieces_new_game(board_t *);
void board_register_pieces_from_board(board_t *, board_t *);
void board_register_piece(board_t *, piece_t *);
piece_t *board_get_piece_by_id(board_t *, piece_id_t);
piece_t *board_get_piece_by_position(board_t *, vector2_t);
uint8_t board_has_piece_on_position(board_t *, vector2_t);
uint8_t board_can_take_position(board_t *, piece_t *, vector2_t);
uint8_t board_is_position_get_attacked(board_t *, side_t, vector2_t);
uint8_t board_is_position_safe_to_move_to(board_t *, side_t, vector2_t);
uint8_t board_is_king_get_attacked(board_t *, side_t);
move_array_t *board_get_moves(board_t *, side_t);
void board_apply_move(board_t *, move_t *);
uint8_t board_is_king_in_danger(board_t *, side_t);
void board_free(board_t *);
void board_debug(board_t *);

#endif