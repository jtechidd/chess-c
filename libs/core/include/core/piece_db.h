#ifndef CH_PIECE_DB_H
#define CH_PIECE_DB_H

#include "core/common.h"
#include "core/piece.h"

typedef struct {
  uint8_t num_pieces;
  ch_piece_id_t cid;
  ch_piece_t pieces[CH_MAX_PIECES];
} ch_piece_db_t;

ch_piece_t *ch_piece_db_create_piece(ch_piece_db_t *pieceDb, ch_side_t side,
                                     ch_piece_type_t type,
                                     ch_vector2_t position,
                                     ch_piece_data_t data,
                                     const ch_piece_methods_t *methods);
ch_piece_t *ch_piece_db_get_by_id(ch_piece_db_t *pieceDb, ch_piece_id_t id);
ch_piece_t *ch_piece_db_get_by_index(ch_piece_db_t *pieceDB, uint8_t idx);
ch_piece_t *ch_piece_db_get_king_by_side(ch_piece_db_t *pieceDb,
                                         ch_side_t side);

#endif