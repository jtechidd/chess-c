#include "core/move_db.h"

ch_move_t *ch_move_db_add_move(ch_move_db_t *move_db, ch_move_t move) {
  if (move_db->total >= CH_MAX_TOTAL_MOVES) {
    return NULL;
  }
  move_db->moves[move_db->total] = move;
  move_db->total++;
  return &move_db->moves[move_db->total - 1];
}

ch_move_t *ch_move_db_get_by_index(ch_move_db_t *move_db, uint16_t idx) {
  if (idx >= 0 && idx < move_db->total) {
    return &move_db->moves[idx];
  }
  return NULL;
}
