#ifndef VECTOR2_H
#define VECTOR2_H

#include <stdint.h>

typedef struct vector2_t {
  int i;
  int j;
} vector2_t;

vector2_t vector2_make(int, int);
vector2_t vector2_add2(vector2_t, vector2_t);
vector2_t vector2_scaled(vector2_t, int);
vector2_t vector2_vflip(vector2_t);
vector2_t vector2_hflip(vector2_t);
int vector2_l1dist(vector2_t, vector2_t);

#endif
