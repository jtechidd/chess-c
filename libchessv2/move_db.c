#include "move_db.h"
#include "defines.h"

#include <string.h>

void move_array_init(move_array_t *move_array) {
  memset(move_array, 0, sizeof(move_array_t));
}

move_t *move_array_add(move_array_t *move_array, move_t *move) {
  if (move_array->length >= MOVE_ARRAY_MAX_LENGTH) {
    return NULL;
  }
  move_memcpy(&move_array->array[move_array->length++], move);
  return &move_array->array[move_array->length - 1];
}

move_t *move_array_get_index(move_array_t *move_array, uint8_t index) {
  if (index >= move_array->length) {
    return NULL;
  }
  return &move_array->array[index];
}

void move_db_init(move_db_t *move_db) {
  memset(move_db, 0, sizeof(move_db_t));
};

uint8_t move_db_get_new_id(move_db_t *move_db) { return ++move_db->cid; }

move_t *move_db_create(move_db_t *move_db, move_t *move) {
  move_t move_with_id;
  move_memcpy(&move_with_id, move);
  move_with_id.id = move_db_get_new_id(move_db);
  return move_array_add(&move_db->moves, &move_with_id);
}

void move_db_compute_index(move_db_t *move_db) {}
