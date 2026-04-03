#ifndef CHESS_MOVE_DB_H
#define CHESS_MOVE_DB_H

#include "defines.h"
#include "move.h"

typedef struct move_db_t move_db_t;
typedef struct move_array_t move_array_t;
typedef struct piece_move_id_array_t piece_move_id_array_t;

struct move_array_t {
  uint8_t length;
  move_t array[MOVE_ARRAY_MAX_LENGTH];
};

void move_array_init(move_array_t *move_array);
move_t *move_array_add(move_array_t *move_array, move_t *move);
move_t *move_array_get_index(move_array_t *move_array, uint8_t index);

struct piece_move_id_array_t {
  uint8_t length;
  uint8_t array[PIECE_MOVE_ID_ARRAY_MAX_LENGTH];
};

void piece_move_id_array_init(piece_move_id_array_t *piece_move_id_array);
void piece_move_id_array_add(piece_move_id_array_t *piece_move_id_array,
                             move_t *move);
uint8_t piece_move_id_array_get_index(piece_move_id_array_t *move_move_id_array,
                                      uint8_t index);

struct move_db_t {
  uint8_t cid;

  move_array_t moves;

  move_t *id_map[MOVE_ARRAY_MAX_LENGTH + 1];
  piece_move_id_array_t piece_move_ids[MAX_NUM_PIECES + 1];
};

void move_db_init(move_db_t *move_db);
uint8_t move_db_get_new_id(move_db_t *move_db);
move_t *move_db_create(move_db_t *move_db, move_t *move);

void move_db_compute_index(move_db_t *move_db);

#endif
