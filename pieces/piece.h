#ifndef PIECE_H
#define PIECE_H

#include <stdint.h>

#include "../move/move_array.h"

typedef struct Board Board;

typedef struct Piece {
    enum PieceId id;
    enum PieceType type;
    enum Side side;
    Vector2 position;
    unsigned int is_captured : 1;

    // Virtual functions
    void (*piece_free)(struct Piece*);
    MoveArray* (*piece_get_positional_moves)(struct Board*, struct Piece*);
} Piece;

uint8_t piece_is_alias(Piece*, Piece*);
uint8_t piece_is_opposite(Piece*, Piece*);

#endif