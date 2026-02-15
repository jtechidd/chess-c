#ifndef MOVE_ARRAY_H
#define MOVE_ARRAY_H

#include <stdlib.h>

#include "move.h"

typedef struct move_array_t {
  size_t capacity;
  size_t length;
  move_t **array;
} move_array_t;

move_array_t *move_array_new();
void move_array_add(move_array_t *, move_t *);
move_t *move_array_get_index(move_array_t *, size_t);
void move_array_debug(move_array_t *);
void move_array_shallow_free(move_array_t *);
void move_array_free(move_array_t *);

#endif