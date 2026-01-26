#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdint.h>

typedef struct vector2_t {
    int8_t i;
    int8_t j;
} vector2_t;

vector2_t vector2_make(int8_t, int8_t);
vector2_t vector2_add2(vector2_t, vector2_t);
vector2_t vector2_scalar_multiply(vector2_t, uint8_t);
vector2_t vector2_horizontal_flip(vector2_t);

#endif
