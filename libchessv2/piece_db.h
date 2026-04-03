#ifndef CHESS_PIECE_DB_H
#define CHESS_PIECE_DB_H

#include "defines.h"
#include "piece.h"

#include <stdint.h>

typedef struct piece_db_t piece_db_t;
typedef struct piece_array_t piece_array_t;
typedef struct piece_id_array_t piece_id_array_t;

struct piece_array_t {
  uint8_t length;
  piece_t array[MAX_NUM_PIECES];
};

piece_t *piece_array_add(piece_array_t *piece_array, piece_t *piece);
piece_t *piece_array_get_index(piece_array_t *piece_array, uint8_t index);

struct piece_id_array_t {
  uint8_t length;
  uint8_t array[MAX_NUM_PIECES];
};

void piece_id_array_add(piece_id_array_t *piece_id_array, uint8_t piece_id);
uint8_t piece_id_array_get_index(piece_id_array_t *piece_ptr_array,
                                 uint8_t index);

struct piece_db_t {
  uint8_t cid;

  piece_array_t pieces;

  piece_t *id_map[MAX_NUM_PIECES + 1];

  piece_id_array_t piece_ids_white;
  uint8_t piece_id_king_white;
  uint8_t piece_id_rook_white_king_side;
  uint8_t piece_id_rook_white_queen_side;

  piece_id_array_t piece_ids_black;
  uint8_t piece_id_king_black;
  uint8_t piece_id_rook_black_king_side;
  uint8_t piece_id_rook_black_queen_side;
};

uint8_t piece_db_get_new_id(piece_db_t *piece_db);
piece_t *piece_db_create(piece_db_t *piece_db, piece_t *piece);
piece_t *piece_db_get_by_id(piece_db_t *piece_db, uint8_t id);
void piece_db_compute_index_standard(piece_db_t *piece_db);

#endif
