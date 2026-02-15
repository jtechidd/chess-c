#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "../utils.h"
#include "move_array.h"

#define INITIAL_CAPACITY 8

move_array_t *move_array_new() {
  move_array_t *move_array = (move_array_t *)xmalloc(sizeof(move_array_t));
  move_array->capacity = INITIAL_CAPACITY;
  move_array->length = 0;
  move_array->array =
      (move_t **)xmalloc(move_array->capacity * sizeof(move_t **));
  return move_array;
}

void move_array_add(move_array_t *move_array, move_t *move) {
  if (move_array->length == move_array->capacity) {
    uint32_t new_capacity = move_array->capacity * 2;
    move_t **reallocated_array = (move_t **)xrealloc(
        move_array->array, new_capacity * sizeof(move_t **));
    move_array->array = reallocated_array;
    move_array->capacity = new_capacity;
  }
  move_array->array[move_array->length] = move;
  move_array->length++;
}

move_t *move_array_get_index(move_array_t *move_array, size_t index) {
  if (index >= move_array->length) {
    return NULL;
  }
  return move_array->array[index];
}

void move_array_debug(move_array_t *move_array) {
  for (size_t i = 0; i < move_array->length; i++) {
    move_t *move = move_array_get_index(move_array, i);
    wprintf(L"move[%ld]: ", i);
    move_debug(move);
  }
}

void move_array_shallow_free(move_array_t *move_array) {
  xfree(move_array->array);
  xfree(move_array);
}

void move_array_free(move_array_t *move_array) {
  for (size_t i = 0; i < move_array->length; i++) {
    xfree(move_array_get_index(move_array, i));
  }
  xfree(move_array->array);
  xfree(move_array);
}
