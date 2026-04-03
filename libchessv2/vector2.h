#ifndef CHESS_VECTOR2_H
#define CHESS_VECTOR2_H

#include <stdint.h>

typedef struct vector2_t vector2_t;
struct vector2_t {
  int8_t i;
  int8_t j;
};

void vector2_init(vector2_t *vector2, int8_t i, int8_t j);
void vector2_memcpy(vector2_t *vector2_dest, vector2_t *vector2_src);
void vector2_add(vector2_t *result, vector2_t *a, vector2_t *b);
void vector2_scaled(vector2_t *result, vector2_t *a, int8_t s);

uint8_t is_position_in_bound(vector2_t *position);

#endif
