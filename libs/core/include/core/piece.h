#ifndef CH_PIECE_H
#define CH_PIECE_H

#include "core/common.h"
#include "core/move.h"
#include "core/move_db.h"
#include "core/vector2.h"

typedef union {
} ch_piece_data_t;

typedef struct _ch_piece_methods_t ch_piece_methods_t;

struct _ch_piece_t {
  ch_piece_id_t id;
  ch_side_t side;
  ch_piece_type_t type;
  ch_vector2_t position;
  uint8_t move_count;
  uint8_t latest_move_turn_count;
  bool is_captured;
  ch_piece_data_t data;
  const ch_piece_methods_t *methods;
};

typedef struct {
  ch_piece_id_t piece_id;
  ch_piece_id_t taking_piece_id;
  ch_piece_id_t castling_rook_id;
  ch_vector2_t castling_rook_position_to;
} ch_apply_move_payload_t;

typedef ch_error_t ch_validate_move_fn_t(ch_piece_t *piece, ch_chess_t *chess,
                                         ch_move_t move,
                                         ch_apply_move_payload_t *payload);
typedef void ch_fill_moves_fn_t(ch_piece_t *piece, ch_chess_t *chess,
                                ch_move_db_t *move_db);

struct _ch_piece_methods_t {
  ch_validate_move_fn_t *validate_move;
  ch_fill_moves_fn_t *fill_moves;
};

void ch_piece_init(ch_piece_t *piece, ch_piece_id_t id, ch_side_t side,
                   ch_piece_type_t type, ch_vector2_t position,
                   ch_piece_data_t data, const ch_piece_methods_t *methods);
ch_validate_move_fn_t ch_piece_validate_move;
ch_fill_moves_fn_t ch_piece_fill_moves;
ch_piece_data_t ch_piece_data_make_empty();

#endif