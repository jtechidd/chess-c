#include "moves.h"

#include <stdio.h>

Move* Move_New(Side side, PieceId piece_id, PieceType piece_type, Position position_from, Position position_to) {
    Move* move = (Move*)malloc(sizeof(Move));

    move->side = side;
    move->piece_id = piece_id;
    move->piece_type = piece_type;
    move->position_from = position_from;
    move->position_to = position_to;

    return move;
}

void Move_Free(Move* move) {
    free(move);
}

MoveArray* MoveArray_New() {
    MoveArray* move_array = (MoveArray*)malloc(sizeof(MoveArray));
    move_array->capacity = 8;
    move_array->length = 0;
    move_array->array = (Move**)malloc(move_array->capacity * sizeof(Move**));
    return move_array;
}

void MoveArray_Add(MoveArray* move_array, Move* move) {
    if (move_array->length == move_array->capacity) {
        uint32_t new_capacity = move_array->capacity * 2;
        Move** reallocated_array = (Move**)realloc(move_array->array, new_capacity * sizeof(Move**));
        if (!reallocated_array) {
            perror("Cannot realloc move array");
        }
        move_array->array = reallocated_array;
        move_array->capacity = new_capacity;
    }
    move_array->array[move_array->length] = move;
    move_array->length++;
}

Move* MoveArray_GetIndex(MoveArray* move_array, size_t index) {
    if (index >= move_array->length) {
        return NULL;
    }
    return move_array->array[index];
}

void MoveArray_Free(MoveArray* move_array) {
    free(move_array);
}
