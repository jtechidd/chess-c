#include "bishop.h"

#include <stddef.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "piece_macros.h"

#define BISHOP_TOTAL_DIRECTIONS 4
#define BISHOP_MAX_SCALE 7
const vector2_t BISHOP_DIRECTIONS[] = {{-1, -1}, {-1, 1}, {1, -1}, {1, 1}};

piece_clone_fn bishop_piece_clone;
piece_get_moves_fn bishop_piece_get_moves;
piece_free_fn bishop_piece_free;

DEFINE_PIECE_NEW_INTERNAL(bishop, BISHOP)
DEFINE_PIECE_CLONE_INTERNAL(bishop)
DEFINE_PIECE_GET_MOVES_INTERNAL(_bishop_get_moves, bishop, BISHOP)
DEFINE_PIECE_FREE_INTERNAL(bishop)

DEFINE_PIECE_NEW(bishop)
DEFINE_PIECE_CAST(bishop, BISHOP)
DEFINE_PIECE_CLONE(bishop)
DEFINE_PIECE_GET_MOVES(bishop)
DEFINE_PIECE_FREE(bishop)

DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(bishop, BISHOP)