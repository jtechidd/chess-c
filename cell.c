#include "cell.h"

Cell Cell_Make(uint8_t has_piece, PieceId piece_id) {
    return (Cell){has_piece, piece_id};
}
