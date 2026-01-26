#include "vector2.h"

vector2_t vector2_make(int8_t i, int8_t j) {
    return (vector2_t){i, j};
}

vector2_t vector2_add2(vector2_t a, vector2_t b) {
    return (vector2_t){a.i + b.i, a.j + b.j};
}

vector2_t vector2_scalar_multiply(vector2_t a, uint8_t s) {
    return (vector2_t){s * a.i, s * a.j};
}

vector2_t vector2_horizontal_flip(vector2_t a) {
    return (vector2_t){-a.i, a.j};
}