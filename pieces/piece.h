#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

#include "../position.h"
#include "../types.h"

typedef struct Piece {
    Position position;
    enum PieceId id;
    enum Side side;
    enum PieceType type;
    unsigned int is_captured : 1;

    // Virtual functions
    void (*Piece_Free)(struct Piece* p);
} Piece;

uint8_t Piece_IsAlias(Piece*, Piece*);
uint8_t Piece_IsOpposite(Piece*, Piece*);

#endif