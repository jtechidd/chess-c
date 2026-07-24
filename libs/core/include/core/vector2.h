#ifndef CH_VECTOR2_H
#define CH_VECTOR2_H

#include "core/common.h"

typedef struct {
  int8_t i;
  int8_t j;
} ch_vector2_t;

ch_vector2_t ch_vector2_make(int8_t i, int8_t j);
bool ch_vector2_equal(ch_vector2_t v1, ch_vector2_t v2);
ch_vector2_t ch_vector2_add(ch_vector2_t v1, ch_vector2_t v2);
ch_vector2_t ch_vector2_sub(ch_vector2_t v1, ch_vector2_t v2);
ch_vector2_t ch_vector2_scalmult(ch_vector2_t v, int8_t s);
ch_vector2_t ch_vector2_inv(ch_vector2_t v);
ch_vector2_t ch_vector2_fliph(ch_vector2_t v);
ch_vector2_t ch_vector2_flipv(ch_vector2_t v);

#endif