#include "queen.h"

#include <stddef.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "piece_macros.h"

#define QUEEN_TOTAL_DIRECTIONS 8
#define QUEEN_MAX_SCALE 7
const vector2_t QUEEN_DIRECTIONS[] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

piece_clone_fn queen_piece_clone;
piece_get_moves_fn queen_piece_get_moves;
piece_free_fn queen_piece_free;

DEFINE_PIECE_NEW_INTERNAL(queen, QUEEN)
DEFINE_PIECE_CLONE_INTERNAL(queen)
DEFINE_PIECE_GET_MOVES_INTERNAL(_queen_get_moves, queen, QUEEN)
DEFINE_PIECE_FREE_INTERNAL(queen)

DEFINE_PIECE_NEW(queen)
DEFINE_PIECE_CAST(queen, QUEEN)
DEFINE_PIECE_CLONE(queen)
DEFINE_PIECE_GET_MOVES(queen)
DEFINE_PIECE_FREE(queen)

DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(queen, QUEEN)