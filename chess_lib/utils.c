#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

#include "board.h"
#include "enums.h"

void *xmalloc(size_t size) {
  void *ptr = malloc(size);
  if (!ptr) {
    fwprintf(stderr, L"Out of memory: malloc(%zu)\n", size);
    exit(EXIT_FAILURE);
  }
  return ptr;
}

void *xrealloc(void *ptr, size_t size) {
  void *new_ptr = realloc(ptr, size);
  if (!new_ptr) {
    fwprintf(stderr, L"Out of memory: realloc(%zu)\n", size);
    exit(EXIT_FAILURE);
  }
  return new_ptr;
}

void xfree(void *ptr) { free(ptr); }

bool is_position_in_bound(vector2_t position) {
  return position.i >= 0 && position.i < BOARD_HEIGHT && position.j >= 0 &&
         position.j < BOARD_WIDTH;
}

bool is_position_top(vector2_t position) { return position.i == 0; }

bool is_position_bottom(vector2_t position) {
  return position.i == BOARD_HEIGHT - 1;
}

bool is_opposite_side(side_t side1, side_t side2) { return side1 != side2; }