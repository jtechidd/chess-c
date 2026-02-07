#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

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

int board_new(board_t **);
int board_register_piece(board_t *, piece_t *);
int board_get_piece_by_id(piece_t **, board_t *, piece_id_t);
int board_get_piece_by_position(piece_t **, board_t *, vector2_t);
bool board_has_piece_on_position(board_t *, vector2_t);
bool board_can_take_position(board_t *, piece_t *, vector2_t);
bool board_is_position_safe_to_move_to(board_t *, side_t, vector2_t);
bool board_is_king_get_attacked(board_t *, side_t);
void board_free(board_t *);
void board_debug(board_t *);

#endif