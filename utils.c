#include "utils.h"

#include "board.h"
#include "enums.h"

bool is_piece_id_valid(piece_id_t piece_id) {
  return piece_id >= 0 && piece_id < TOTAL_PIECES;
}

bool is_position_in_bound(vector2_t position) {
  return position.i >= 0 && position.i < BOARD_HEIGHT && position.j >= 0 &&
         position.j < BOARD_WIDTH;
}

bool is_position_top(vector2_t position) { return position.i == 0; }

bool is_position_bottom(vector2_t position) {
  return position.i == BOARD_HEIGHT - 1;
}

bool is_side_valid(side_t side) { return side >= 0 && side < TOTAL_SIDES; }

bool is_opposite_side(side_t side1, side_t side2) { return side1 != side2; }