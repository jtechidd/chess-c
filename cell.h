#ifndef CELL_H
#define CELL_H

#include <stdbool.h>

#include "./enums.h"

typedef struct cell_t {
  bool has_piece;
  piece_id_t piece_id;
} cell_t;

cell_t cell_make(bool has_piece, piece_id_t piece_id);

#endif