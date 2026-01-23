#ifndef MOVES_H
#define MOVES_H

#include <stdlib.h>

#include "position.h"
#include "types.h"

typedef struct Move {
    Side side;
    PieceId piece_id;
    PieceType piece_type;
    Position position_from;
    Position position_to;
} Move;

Move* Move_New(Side, PieceId, PieceType, Position, Position);
void Move_Free(Move*);

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
