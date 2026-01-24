#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

#include "../move/move_array.h"

typedef struct Board Board;

typedef struct Piece {
    enum PieceId id;
    enum PieceType type;
    enum Side side;
    Position position;
    unsigned int is_captured : 1;

    // Virtual functions
    void (*Piece_Free)(struct Piece*);
    MoveArray* (*Piece_GetPositionalMoves)(struct Board*, struct Piece*);
} Piece;

uint8_t Piece_IsAlias(Piece*, Piece*);
uint8_t Piece_IsOpposite(Piece*, Piece*);

#endif