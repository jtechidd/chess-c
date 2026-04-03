#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#include "vector2.h"
#include <stdint.h>

typedef struct piece_t piece_t;
typedef struct move_t move_t;

struct move_t {
  uint8_t id;
  uint8_t flags;
  uint8_t piece_id;
  vector2_t position_from;
  vector2_t position_to;
  uint8_t take_piece_id;
  uint8_t promote_to;
  uint8_t castle_type;
};

void move_init_move_piece(move_t *move, piece_t *piece, vector2_t *position_to);
void move_init_take_piece(move_t *move, piece_t *piece, vector2_t *position_to,
                          piece_t *take_piece);
void move_init_castle(move_t *move, uint8_t castle_type);
void move_with_promotion(move_t *move, uint8_t promote_to);

void move_memcpy(move_t *dest, move_t *src);

#endif
