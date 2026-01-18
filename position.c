#include "position.h"

#include "board.h"

uint8_t Position_OutOfBound(Position* pos) {
    return !(pos->i >= 0 && pos->i < BOARD_HEIGHT && pos->j >= 0 &&
             pos->j < BOARD_WIDTH);
}

Position Position_Make(int i, int j) {
    return (Position){i, j};
}