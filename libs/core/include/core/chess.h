#ifndef CH_CHESS_H
#define CH_CHESS_H

#include "core/common.h"
#include "core/move.h"
#include "core/piece_db.h"
#include "core/vector2.h"

typedef struct {
  ch_piece_id_t piece_id;
  bool has_piece;
} ch_cell_t;

typedef struct {
  ch_cell_t table[CH_BOARD_HEIGHT][CH_BOARD_WIDTH];
} ch_board_t;

struct _ch_chess_t {
  ch_side_t turn;
  uint8_t num_turns;
  ch_board_t board;
  ch_piece_db_t piece_db;
};

void ch_chess_spawn_piece(ch_chess_t *chess, ch_side_t side,
                          ch_piece_type_t type, ch_vector2_t position,
                          ch_piece_data_t data,
                          const ch_piece_methods_t *methods);
ch_piece_t *ch_chess_get_piece_on_position(ch_chess_t *chess,
                                           ch_vector2_t position);
bool ch_chess_is_position_safe(ch_chess_t *chess, ch_vector2_t position);
void ch_chess_init_standard(ch_chess_t *chess);
ch_error_t ch_chess_apply_move(ch_chess_t *chess, ch_move_t move);
ch_error_t ch_chess_apply_move_lan(ch_chess_t *chess,
                                   const char *move_notation);
void ch_chess_print_board(ch_chess_t *chess);

#endif