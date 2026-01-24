#ifndef CELL_H
#define CELL_H

#include "./enums.h"

typedef struct Cell {
    unsigned int has_piece : 1;
    PieceId piece_id;
} Cell;

Cell Cell_Make(uint8_t has_piece, PieceId piece_id);
#endif