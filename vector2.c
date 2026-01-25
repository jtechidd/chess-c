#include "vector2.h"

Vector2 vector2_make(int8_t i, int8_t j) {
    return (Vector2){i, j};
}

Vector2 vector2_add2(Vector2 a, Vector2 b) {
    return (Vector2){a.i + b.i, a.j + b.j};
}

Vector2 vector2_scalar_multiply(Vector2 a, uint8_t s) {
    return (Vector2){s * a.i, s * a.j};
}

Vector2 vector2_horizontal_flip(Vector2 a) {
    return (Vector2){-a.i, a.j};
}