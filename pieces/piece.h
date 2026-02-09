#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>

#include "../enums.h"
#include "../vector2.h"

typedef struct board_t board_t;
typedef struct piece_t piece_t;
typedef struct move_array_t move_array_t;

typedef int piece_new_fn(piece_t **, piece_id_t, side_t, vector2_t);
typedef int piece_clone_fn(piece_t **, piece_t *);
typedef int piece_get_moves_fn(move_array_t **, piece_t *, board_t *);
typedef int piece_free_fn(piece_t *);

#define WUR __attribute__((warn_unused_result()))

typedef struct piece_t {
  enum piece_id_t id;
  enum piece_type_t type;
  enum side_t side;
  vector2_t position;
  unsigned int is_captured : 1;
  unsigned int moving_count;

  // Virtual functions
  WUR piece_clone_fn *piece_clone;
  WUR piece_get_moves_fn *piece_get_moves;
  piece_free_fn *piece_free;
} piece_t;

bool piece_is_opposite(piece_t *, piece_t *);

typedef int board_is_position_get_attacked_by_piece_fn(bool *, board_t *, side_t, vector2_t);

#undef WUR

#endif