#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

#include "../move/move_array.h"

typedef struct Board Board;
typedef struct Piece Piece;

typedef void (*PieceFreeFn)(struct Piece*);
typedef MoveArray* (*PieceGetPositionalMovesFn)(Piece*, Board*);

typedef struct Piece {
    enum PieceId id;
    enum PieceType type;
    enum Side side;
    Vector2 position;
    unsigned int is_captured : 1;

    // Virtual functions
    PieceFreeFn piece_free;
    PieceGetPositionalMovesFn piece_get_positional_moves;
} Piece;

uint8_t piece_is_opposite(Piece*, Piece*);

#endif