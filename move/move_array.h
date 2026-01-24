#ifndef MOVE_ARRAY_H
#define MOVE_ARRAY_H

#include <stdlib.h>

#include "move.h"

typedef struct MoveArray {
    size_t capacity;
    size_t length;
    Move** array;
} MoveArray;

MoveArray* MoveArray_New();
void MoveArray_Add(MoveArray*, Move*);
Move* MoveArray_GetIndex(MoveArray*, size_t);
void MoveArray_Free(MoveArray*);

#endif