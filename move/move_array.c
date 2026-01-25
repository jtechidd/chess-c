#include "move_array.h"

#include <stdio.h>

MoveArray* move_array_new() {
    MoveArray* move_array = (MoveArray*)malloc(sizeof(MoveArray));
    move_array->capacity = 8;
    move_array->length = 0;
    move_array->array = (Move**)malloc(move_array->capacity * sizeof(Move**));
    return move_array;
}

void move_array_add(MoveArray* move_array, Move* move) {
    if (move_array->length == move_array->capacity) {
        uint32_t new_capacity = move_array->capacity * 2;
        Move** reallocated_array = (Move**)realloc(move_array->array, new_capacity * sizeof(Move**));
        if (!reallocated_array) {
            perror("Cannot realloc move array");
            exit(EXIT_FAILURE);
        }
        move_array->array = reallocated_array;
        move_array->capacity = new_capacity;
    }
    move_array->array[move_array->length] = move;
    move_array->length++;
}

Move* move_array_get_index(MoveArray* move_array, size_t index) {
    if (index >= move_array->length) {
        return NULL;
    }
    return move_array->array[index];
}

void move_array_debug(MoveArray* move_array) {
    for (size_t i = 0; i < move_array->length; i++) {
        Move* move = move_array_get_index(move_array, i);
        printf("move[%ld]: ", i);
        move_debug(move);
    }
}

void move_array_shallow_free(MoveArray* move_array) {
    free(move_array);
}

void move_array_free(MoveArray* move_array) {
    for (size_t i = 0; i < move_array->length; i++) {
        free(move_array_get_index(move_array, i));
    }
    free(move_array);
}
