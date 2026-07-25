
#include "core/piece_db.h"
#include "core/common.h"
#include "core/piece.h"

ch_piece_t *ch_piece_db_create_piece(ch_piece_db_t *piece_db, ch_side_t side,
                                     ch_piece_type_t type,
                                     ch_vector2_t position,
                                     ch_piece_data_t data,
                                     const ch_piece_methods_t *methods) {
  piece_db->cid++;
  ch_piece_init(&piece_db->pieces[piece_db->total], piece_db->cid, side,
                type, position, data, methods);
  piece_db->total++;
  return &piece_db->pieces[piece_db->total - 1];
}

ch_piece_t *ch_piece_db_get_by_id(ch_piece_db_t *piece_db, ch_piece_id_t id) {
  for (uint8_t i = 0; i < piece_db->total; i++) {
    if (piece_db->pieces[i].id == id) {
      return &piece_db->pieces[i];
    }
  }
  return NULL;
}

ch_piece_t *ch_piece_db_get_by_index(ch_piece_db_t *piece_db, uint8_t idx) {
  if (idx >= 0 && idx < piece_db->total) {
    return &piece_db->pieces[idx];
  }
  return NULL;
}

ch_piece_t *ch_piece_db_get_king_by_side(ch_piece_db_t *piece_db,
                                         ch_side_t side) {
  for (uint8_t i = 0; i < piece_db->total; i++) {
    if (piece_db->pieces[i].side == side &&
        piece_db->pieces[i].type == CH_PIECE_TYPE_KING) {
      return &piece_db->pieces[i];
    }
  }
  return NULL;
}
