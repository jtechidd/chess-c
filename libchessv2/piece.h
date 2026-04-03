#ifndef CHESS_PIECE_H
#define CHESS_PIECE_H

#include <stdint.h>

#include "vector2.h"

typedef struct chess_t chess_t;
typedef struct piece_t piece_t;
typedef union piece_data_t piece_data_t;
typedef struct piece_methods_t piece_methods_t;

union piece_data_t {
  struct {
    uint8_t type;
  } rook;
};

void piece_data_init_empty(piece_data_t *piece_data);
void piece_data_init_rook(piece_data_t *piece_data, uint8_t type);
void piece_data_memcpy(piece_data_t *dest, piece_data_t *src);

struct piece_t {
  uint8_t id;
  uint8_t type;
  uint8_t side;
  uint8_t is_captured;
  uint8_t move_count;
  vector2_t position;
  piece_data_t data;
};

void piece_init(piece_t *piece, uint8_t type, uint8_t side, vector2_t *position,
                piece_data_t *data);
uint8_t piece_is_opposite(piece_t *piece_a, piece_t *piece_b);
void piece_memcpy(piece_t *dest, piece_t *src);

struct piece_methods_t {
  void (*piece_fill_moves)(chess_t *chess, piece_t *piece);
};

extern const piece_methods_t PIECE_METHODS_ROOK;
extern const piece_methods_t PIECE_METHODS_KNIGHT;
extern const piece_methods_t PIECE_METHODS_BISHOP;
extern const piece_methods_t PIECE_METHODS_QUEEN;
extern const piece_methods_t PIECE_METHODS_KING;
extern const piece_methods_t PIECE_METHODS_PAWN;

void piece_fill_moves(chess_t *chess, piece_t *piece);

#endif
