#include "vector2.h"
#include "defines.h"

#include <string.h>

void vector2_init(vector2_t *vector2, int8_t i, int8_t j) {
  vector2->i = i;
  vector2->j = j;
}

void vector2_memcpy(vector2_t *vector2_dest, vector2_t *vector2_src) {
  memcpy(vector2_dest, vector2_src, sizeof(vector2_t));
}

void vector2_add(vector2_t *result, vector2_t *a, vector2_t *b) {
  result->i = a->i + b->i;
  result->j = a->j + b->j;
}

void vector2_scaled(vector2_t *result, vector2_t *a, int8_t s) {
  result->i = a->i * s;
  result->j = a->j * s;
}

uint8_t is_position_in_bound(vector2_t *position) {
  return position->i >= 0 && position->j < BOARD_HEIGHT && position->j >= 0 &&
         position->j < BOARD_WIDTH;
}
