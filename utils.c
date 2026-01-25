#include "utils.h"

#include "board.h"

uint8_t is_piece_id_valid(PieceId piece_id) {
    return piece_id >= 0 && piece_id < TOTAL_PIECES;
}

uint8_t is_position_in_boundary(Vector2 position) {
    return position.i >= 0 && position.i < BOARD_HEIGHT && position.j >= 0 && position.j < BOARD_WIDTH;
}

uint8_t is_position_top(Vector2 position) {
    return position.i == 0;
}

uint8_t is_position_bottom(Vector2 position) {
    return position.i == BOARD_HEIGHT - 1;
}

uint8_t is_opposite_side(Side side1, Side side2) {
    return side1 != side2;
}