#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdint.h>

typedef struct Vector2 {
    int8_t i;
    int8_t j;
} Vector2;

Vector2 vector2_make(int8_t, int8_t);
Vector2 vector2_add2(Vector2, Vector2);
Vector2 vector2_scalar_multiply(Vector2, uint8_t);
Vector2 vector2_horizontal_flip(Vector2);

#endif
