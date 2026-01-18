#include "piece.h"

uint8_t Piece_Alias(Piece* p1, Piece* p2) { return p1->side == p2->side; }
uint8_t Piece_IsOpposite(Piece* p1, Piece* p2) { return p1->side != p2->side; }
