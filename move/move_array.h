#ifndef MOVE_ARRAY_H
#define MOVE_ARRAY_H

#include <stdlib.h>

#include "move.h"

typedef struct MoveArray {
    size_t capacity;
    size_t length;
    Move** array;
} MoveArray;

MoveArray* move_array_new();
void move_array_add(MoveArray*, Move*);
Move* move_array_get_index(MoveArray*, size_t);
void move_array_debug(MoveArray*);
void move_array_free(MoveArray*);

#endif