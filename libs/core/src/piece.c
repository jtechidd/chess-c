#include <assert.h>

#include "core/common.h"
#include "core/piece.h"

void ch_piece_init(ch_piece_t *piece, ch_piece_id_t id, ch_side_t side,
                   ch_piece_type_t type, ch_vector2_t position,
                   ch_piece_data_t data, const ch_piece_methods_t *methods) {
  memset(piece, 0, sizeof(ch_piece_t));

  piece->id = id;
  piece->side = side;
  piece->type = type;
  piece->position = position;
  piece->move_count = 0;
  piece->latest_move_turn_num = 0;
  piece->is_captured = false;
  piece->data = data;
  piece->methods = methods;
}

ch_error_t ch_piece_validate_move(ch_piece_t *piece, ch_chess_t *chess,
                                  ch_move_t move, ch_validate_move_out_t *out) {
  assert(piece->methods && piece->methods->validate_move);
  return piece->methods->validate_move(piece, chess, move, out);
}

ch_piece_data_t ch_piece_data_make_empty() { return (ch_piece_data_t){}; }