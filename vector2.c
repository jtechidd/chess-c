#include "vector2.h"

#include <stdlib.h>

vector2_t vector2_make(int8_t i, int8_t j) { return (vector2_t){i, j}; }

vector2_t vector2_add2(vector2_t a, vector2_t b) { return vector2_make(a.i + b.i, a.j + b.j); }

vector2_t vector2_scaled(vector2_t a, uint8_t s) { return vector2_make(s * a.i, s * a.j); }

vector2_t vector2_vflip(vector2_t a) { return vector2_make(-a.i, a.j); }

vector2_t vector2_hflip(vector2_t a) { return vector2_make(a.i, -a.j); }

uint8_t vector2_l1dist(vector2_t a, vector2_t b) { return abs(a.i - b.i) + abs(a.j - b.j); }