#include "utils.h"

#include "board.h"
#include "enums.h"

uint8_t is_piece_id_valid(piece_id_t piece_id) { return piece_id >= 0 && piece_id < TOTAL_PIECES; }

uint8_t is_position_in_bound(vector2_t position) { return position.i >= 0 && position.i < BOARD_HEIGHT && position.j >= 0 && position.j < BOARD_WIDTH; }

uint8_t is_position_top(vector2_t position) { return position.i == 0; }

uint8_t is_position_bottom(vector2_t position) { return position.i == BOARD_HEIGHT - 1; }

uint8_t is_opposite_side(side_t side1, side_t side2) { return side1 != side2; }

void get_all_piece_ids_range_by_side(side_t side, piece_id_t *piece_id_start, piece_id_t *piece_id_end) {
  if (side == SIDE_WHITE) {
    *piece_id_start = PIECE_ID_WHITE_ROOK_1;
    *piece_id_end = PIECE_ID_WHITE_PAWN_8;
  } else if (side == SIDE_BLACK) {
    *piece_id_start = PIECE_ID_BLACK_ROOK_1;
    *piece_id_end = PIECE_ID_BLACK_PAWN_8;
  }
}

piece_id_t get_king_piece_id_by_side(side_t side) {
  if (side == SIDE_WHITE) {
    return PIECE_ID_WHITE_KING;
  } else if (side == SIDE_BLACK) {
    return PIECE_ID_BLACK_KING;
  }
}