#ifndef CH_COMMON_H
#define CH_COMMON_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CH_BOARD_WIDTH 8
#define CH_BOARD_HEIGHT 8
#define CH_MAX_PIECES 64
#define CH_EMPTY 0
#define CH_NUM_PIECE_TYPES 6
#define CH_NUM_SIDES 2
#define CH_NUM_ROOK_TYPES 2

typedef uint8_t CH_PieceId;

typedef enum : uint8_t {
  CH_ERR_SUCCESS = 0,
  CH_ERR_INVALID_NOTATION,
  CH_ERR_ILLEGAL_MOVE
} CH_Error;

typedef enum : uint8_t { CH_SIDE_WHITE = 1, CH_SIDE_BLACK } CH_Side;

typedef enum : uint8_t {
  CH_PIECE_TYPE_PAWN = 1,
  CH_PIECE_TYPE_ROOK,
  CH_PIECE_TYPE_KNIGHT,
  CH_PIECE_TYPE_BISHOP,
  CH_PIECE_TYPE_QUEEN,
  CH_PIECE_TYPE_KING,
} CH_PieceType;

typedef enum : uint8_t {
  CH_ROOK_TYPE_QUEEN_SIDE = 1,
  CH_ROOK_TYPE_KING_SIDE
} CH_RookType;

typedef struct CH_Chess CH_Chess;

#endif