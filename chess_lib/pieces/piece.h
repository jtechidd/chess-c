#ifndef PIECE_H
#define PIECE_H

#include <stdbool.h>
#include <wchar.h>

#include "../enums.h"
#include "../vector2.h"

typedef struct board_t board_t;
typedef struct piece_t piece_t;
typedef struct move_array_t move_array_t;

typedef void piece_free_fn(piece_t *);
typedef move_array_t *piece_get_moves_fn(piece_t *, board_t *);
typedef piece_t *piece_clone_fn(piece_t *);
typedef wchar_t piece_get_wchar_fn(piece_t *);

typedef struct piece_t {
  enum piece_id_t id;
  enum piece_type_t type;
  enum side_t side;
  vector2_t position;
  unsigned int is_captured : 1;
  unsigned int moving_count;

  piece_clone_fn *piece_clone;
  piece_free_fn *piece_free;
  piece_get_moves_fn *piece_get_moves;
  piece_get_wchar_fn *piece_get_wchar;
} piece_t;

bool piece_is_opposite(piece_t *, piece_t *);

typedef bool board_is_position_get_attacked_by_piece_fn(board_t *, side_t,
                                                        vector2_t);

#endif