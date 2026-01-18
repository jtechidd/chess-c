#ifndef TYPE_H
#define TYPE_H

#include <stdint.h>

typedef enum Side : uint8_t { Side_White, Side_Black } Side;

typedef enum PieceType : uint8_t {
    PieceType_Rook,
    PieceType_Knight,
    PieceType_Bishop,
    PieceType_Queen,
    PieceType_King,
    PieceType_Pawn
} PieceType;

typedef enum PieceId : uint8_t {
    PieceId_BlackRook1,
    PieceId_BlackKnight1,
    PieceId_BlackBishop1,
    PieceId_BlackQueen,
    PieceId_BlackKing,
    PieceId_BlackBishop2,
    PieceId_BlackKnight2,
    PieceId_BlackRook2,
    PieceId_BlackPawn1,
    PieceId_BlackPawn2,
    PieceId_BlackPawn3,
    PieceId_BlackPawn4,
    PieceId_BlackPawn5,
    PieceId_BlackPawn6,
    PieceId_BlackPawn7,
    PieceId_BlackPawn8,
    PieceId_WhiteRook1,
    PieceId_WhiteKnight1,
    PieceId_WhiteBishop1,
    PieceId_WhiteQueen,
    PieceId_WhiteKing,
    PieceId_WhiteBishop2,
    PieceId_WhiteKnight2,
    PieceId_WhiteRook2,
    PieceId_WhitePawn1,
    PieceId_WhitePawn2,
    PieceId_WhitePawn3,
    PieceId_WhitePawn4,
    PieceId_WhitePawn5,
    PieceId_WhitePawn6,
    PieceId_WhitePawn7,
    PieceId_WhitePawn8,
} PieceId;

uint8_t PieceId_IsValid(PieceId);

#endif