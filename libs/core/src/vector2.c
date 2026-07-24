#include "core/vector2.h"

ch_vector2_t ch_vector2_make(int8_t i, int8_t j) { return (ch_vector2_t){i, j}; }

bool ch_vector2_equal(ch_vector2_t v1, ch_vector2_t v2) {
  return v1.i == v2.i && v1.j == v2.j;
}

ch_vector2_t ch_vector2_add(ch_vector2_t v1, ch_vector2_t v2) {
  return ch_vector2_make(v1.i + v2.i, v1.j + v2.j);
}

ch_vector2_t ch_vector2_sub(ch_vector2_t v1, ch_vector2_t v2) {
  return ch_vector2_make(v1.i - v2.i, v1.j - v2.j);
}

ch_vector2_t ch_vector2_scalmult(ch_vector2_t v, int8_t s) {
  return ch_vector2_make(v.i * s, v.j * s);
}

ch_vector2_t ch_vector2_inv(ch_vector2_t v) { return ch_vector2_scalmult(v, -1); }

ch_vector2_t ch_vector2_fliph(ch_vector2_t v) { return ch_vector2_make(v.i, -v.j); }

ch_vector2_t ch_vector2_flipv(ch_vector2_t v) { return ch_vector2_make(-v.i, v.j); }
