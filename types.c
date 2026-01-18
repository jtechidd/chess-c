#include "types.h"

#include "board.h"

uint8_t PieceId_IsValid(PieceId id) { return id >= 0 && id < TOTAL_PIECES; }