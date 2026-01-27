#ifndef CELL_H
#define CELL_H

#include "./enums.h"

typedef struct cell_t {
  unsigned int has_piece : 1;
  piece_id_t piece_id;
} cell_t;

cell_t cell_make(uint8_t has_piece, piece_id_t piece_id);
#endif