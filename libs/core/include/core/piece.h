#ifndef CH_PIECE_H
#define CH_PIECE_H

#include "core/common.h"
#include "core/move.h"
#include "core/vector2.h"

typedef union {
} ch_piece_data_t;

typedef struct _ch_piece_methods_t ch_piece_methods_t;

typedef struct {
  ch_piece_id_t id;
  ch_side_t side;
  ch_piece_type_t type;
  ch_vector2_t position;
  uint8_t move_count;
  uint8_t latest_move_turn_num;
  bool is_captured;
  ch_piece_data_t data;
  const ch_piece_methods_t *methods;
} ch_piece_t;

typedef struct {
  ch_piece_t *piece_taking;
  ch_piece_t *piece_castling_rook;
  ch_vector2_t piece_castling_rook_position_to;
} ch_validate_move_out_t;

typedef ch_error_t ch_validate_move_fn_t(ch_piece_t *piece, ch_chess_t *chess,
                                         ch_move_t move,
                                         ch_validate_move_out_t *out);

struct _ch_piece_methods_t {
  ch_validate_move_fn_t *validate_move;
};

void ch_piece_init(ch_piece_t *piece, ch_piece_id_t id, ch_side_t side,
                   ch_piece_type_t type, ch_vector2_t position,
                   ch_piece_data_t data, const ch_piece_methods_t *methods);
ch_validate_move_fn_t ch_piece_validate_move;
ch_piece_data_t ch_piece_data_make_empty();

#endif