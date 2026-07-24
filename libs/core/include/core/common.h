#ifndef CH_COMMON_H
#define CH_COMMON_H

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CH_BOARD_SIZE 8
#define CH_BOARD_WIDTH CH_BOARD_SIZE
#define CH_BOARD_HEIGHT CH_BOARD_SIZE
#define CH_MAX_PIECES 64
#define CH_EMPTY 0
#define CH_NUM_PIECE_TYPES 6
#define CH_NUM_SIDES 2

typedef uint8_t ch_piece_id_t;

typedef enum : uint8_t {
  CH_ERR_SUCCESS = 0,
  CH_ERR_INVALID_NOTATION,
  CH_ERR_ILLEGAL_MOVE
} ch_error_t;

typedef enum : uint8_t { CH_SIDE_WHITE = 1, CH_SIDE_BLACK } ch_side_t;

typedef enum : uint8_t {
  CH_PIECE_TYPE_PAWN = 1,
  CH_PIECE_TYPE_ROOK,
  CH_PIECE_TYPE_KNIGHT,
  CH_PIECE_TYPE_BISHOP,
  CH_PIECE_TYPE_QUEEN,
  CH_PIECE_TYPE_KING,
} ch_piece_type_t;

typedef struct _ch_chess_t ch_chess_t;

#endif