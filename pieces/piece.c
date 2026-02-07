#include "piece.h"

#include "../utils.h"

bool piece_is_opposite(piece_t *piece_a, piece_t *piece_b) {
  return is_opposite_side(piece_a->side, piece_b->side);
}
