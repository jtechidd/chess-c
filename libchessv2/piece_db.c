#include "piece_db.h"

#include <stddef.h>
#include <string.h>

piece_t *piece_array_add(piece_array_t *piece_array, piece_t *piece) {
  if (piece_array->length >= MAX_NUM_PIECES) {
    return NULL;
  }
  piece_memcpy(&piece_array->array[piece_array->length++], piece);
  return &piece_array->array[piece_array->length - 1];
}

piece_t *piece_array_get_index(piece_array_t *piece_array, uint8_t index) {
  if (index >= MAX_NUM_PIECES) {
    return NULL;
  }
  return &piece_array->array[index];
}

void piece_id_array_add(piece_id_array_t *piece_id_array, uint8_t piece_id) {
  if (piece_id_array->length >= MAX_NUM_PIECES) {
    return;
  }
  piece_id_array->array[piece_id_array->length++] = piece_id;
}

uint8_t piece_id_array_get_index(piece_id_array_t *piece_ptr_array,
                                 uint8_t index) {
  if (piece_ptr_array->length >= MAX_NUM_PIECES) {
    return 0;
  }
  return piece_ptr_array->array[index];
}

uint8_t piece_db_get_new_id(piece_db_t *piece_db) { return ++piece_db->cid; }

piece_t *piece_db_create(piece_db_t *piece_db, piece_t *piece) {
  piece_t piece_with_id;
  piece_memcpy(&piece_with_id, piece);
  piece_with_id.id = piece_db_get_new_id(piece_db);
  return piece_array_add(&piece_db->pieces, &piece_with_id);
}

piece_t *piece_db_get_by_id(piece_db_t *piece_db, uint8_t piece_id) {
  return piece_db->id_map[piece_id];
}

void piece_db_compute_index_standard(piece_db_t *piece_db) {
  for (uint8_t i = 0; i < piece_db->pieces.length; i++) {
    piece_t *piece = piece_array_get_index(&piece_db->pieces, i);
    piece_db->id_map[piece->id] = piece;

    if (piece->side == SIDE_WHITE) {
      piece_id_array_add(&piece_db->piece_ids_white, piece->id);
      if (piece->type == PIECE_TYPE_KING) {
        piece_db->piece_id_king_white = piece->id;
      } else if (piece->type == PIECE_TYPE_ROOK) {
        if (piece->data.rook.type == ROOK_TYPE_KING_SIDE) {
          piece_db->piece_id_rook_white_king_side = piece->id;
        } else if (piece->data.rook.type == ROOK_TYPE_QUEEN_SIDE) {
          piece_db->piece_id_rook_white_queen_side = piece->id;
        }
      }
    } else if (piece->side == SIDE_BLACK) {
      piece_id_array_add(&piece_db->piece_ids_black, piece->id);
      if (piece->type == PIECE_TYPE_KING) {
        piece_db->piece_id_king_black = piece->id;
      } else if (piece->type == PIECE_TYPE_ROOK) {
        if (piece->data.rook.type == ROOK_TYPE_KING_SIDE) {
          piece_db->piece_id_rook_black_king_side = piece->id;
        } else if (piece->data.rook.type == ROOK_TYPE_QUEEN_SIDE) {
          piece_db->piece_id_rook_black_queen_side = piece->id;
        }
      }
    }
  }
}