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

#define WUR __attribute__((warn_unused_result()))

WUR int board_new(board_t **);
WUR int board_register_piece(board_t *, piece_t *);
WUR int board_get_moves(move_array_t **, board_t *, side_t);
WUR int board_apply_move(board_t *, move_t *);
WUR int board_get_piece_by_id(piece_t **, board_t *, piece_id_t);
WUR int board_get_piece_by_position(piece_t **, board_t *, vector2_t);
WUR int board_has_piece_on_position(bool *, board_t *, vector2_t);
WUR int board_can_take_position(bool *, board_t *, piece_t *, vector2_t);
WUR int board_is_position_safe_to_move_to(bool *, board_t *, side_t, vector2_t);
WUR int board_is_king_get_attacked(bool *, board_t *, side_t);
int board_free(board_t *);
int board_debug(board_t *);

#undef WUR

#endif