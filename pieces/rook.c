#include "rook.h"

#include <stddef.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "piece_macros.h"

#define ROOK_TOTAL_DIRECTIONS 4
#define ROOK_MAX_SCALE 7
const vector2_t ROOK_DIRECTIONS[] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};

piece_clone_fn rook_piece_clone;
piece_get_moves_fn rook_piece_get_moves;
piece_free_fn rook_piece_free;

DEFINE_PIECE_NEW_INTERNAL(rook, ROOK)
DEFINE_PIECE_CLONE_INTERNAL(rook)
DEFINE_PIECE_GET_MOVES_INTERNAL(_rook_get_moves, rook, ROOK)
DEFINE_PIECE_FREE_INTERNAL(rook)

DEFINE_PIECE_NEW(rook)
DEFINE_PIECE_CAST(rook, ROOK)
DEFINE_PIECE_CLONE(rook)
DEFINE_PIECE_GET_MOVES(rook)
DEFINE_PIECE_FREE(rook)

DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(rook, ROOK)
