#ifndef CH_MOVE_H
#define CH_MOVE_H

#include "core/common.h"
#include "core/vector2.h"

typedef struct {
  ch_piece_type_t piece_type;
  ch_vector2_t position_from;
  bool is_taking;
  ch_vector2_t position_to;
  ch_piece_type_t promote_to;
} ch_move_t;

ch_error_t ch_move_parse_lan(ch_move_t *move, const char *notation);
void ch_move_to_str(ch_move_t *move, char *buf, uint8_t buf_len);
ch_move_t ch_move_make_from_piece(ch_piece_t *piece, bool is_taking,
                                  ch_vector2_t position_to,
                                  ch_piece_type_t promote_to);

#endif