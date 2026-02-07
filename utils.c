#include "utils.h"

#include "board.h"
#include "enums.h"

uint8_t is_piece_id_valid(piece_id_t piece_id) {
  return piece_id >= 0 && piece_id < TOTAL_PIECES;
}

uint8_t is_position_in_bound(vector2_t position) {
  return position.i >= 0 && position.i < BOARD_HEIGHT && position.j >= 0 &&
         position.j < BOARD_WIDTH;
}

uint8_t is_position_top(vector2_t position) { return position.i == 0; }

uint8_t is_position_bottom(vector2_t position) {
  return position.i == BOARD_HEIGHT - 1;
}

uint8_t is_opposite_side(side_t side1, side_t side2) { return side1 != side2; }