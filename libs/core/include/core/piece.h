#ifndef CH_PIECE_H
#define CH_PIECE_H

#include "core/common.h"
#include "core/move.h"
#include "core/vector2.h"

typedef union {
  struct {
    ch_rook_type_t type;
  } rook;
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

struct _ch_piece_methods_t {
  ch_error_t (*validate_move)(ch_piece_t *piece, ch_chess_t *chess,
                              ch_move_t move, ch_piece_t **taking_piece);
};

void ch_piece_init(ch_piece_t *piece, ch_piece_id_t id, ch_side_t side,
                   ch_piece_type_t type, ch_vector2_t position,
                   ch_piece_data_t data, const ch_piece_methods_t *methods);
ch_error_t ch_piece_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                  ch_move_t move, ch_piece_t **taking_piece);
ch_piece_data_t ch_piece_data_make_empty();

#endif