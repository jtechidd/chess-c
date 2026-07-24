
#include "core/piece_db.h"
#include "core/common.h"
#include "core/piece.h"

ch_piece_t *ch_piece_db_create_piece(ch_piece_db_t *pieceDb, ch_side_t side,
                                 ch_piece_type_t type, ch_vector2_t position,
                                 ch_piece_data_t data, const ch_piece_methods_t *methods) {
  pieceDb->cid++;
  ch_piece_init(&pieceDb->pieces[pieceDb->num_pieces], pieceDb->cid, side, type,
                position, data, methods);
  pieceDb->num_pieces++;
  return &pieceDb->pieces[pieceDb->num_pieces - 1];
}

ch_piece_t *ch_piece_db_get_by_id(ch_piece_db_t *pieceDb, ch_piece_id_t id) {
  for (uint8_t i = 0; i < pieceDb->num_pieces; i++) {
    if (pieceDb->pieces[i].id == id) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

ch_piece_t *ch_piece_db_get_by_index(ch_piece_db_t *pieceDb, uint8_t idx) {
  if (idx >= 0 && idx < pieceDb->num_pieces) {
    return &pieceDb->pieces[idx];
  }
  return NULL;
}

ch_piece_t *ch_piece_db_get_king_by_side(ch_piece_db_t *pieceDb, ch_side_t side) {
  for (uint8_t i = 0; i < pieceDb->num_pieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_KING) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

ch_piece_t *ch_piece_db_get_rook_by_side_and_type(ch_piece_db_t *pieceDb, ch_side_t side,
                                           ch_rook_type_t type) {
  for (uint8_t i = 0; i < pieceDb->num_pieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_ROOK &&
        pieceDb->pieces[i].data.rook.type == type) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}
