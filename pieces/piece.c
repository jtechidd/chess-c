#include "piece.h"

#include "../utils.h"

uint8_t piece_is_opposite(Piece* p1, Piece* p2) {
    return is_opposite_side(p1->side, p2->side);
}
