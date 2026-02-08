#include "knight.h"

#include <stddef.h>

#include "../board.h"
#include "../errno.h"
#include "../move/move.h"
#include "../move/move_array.h"
#include "../utils.h"
#include "piece.h"
#include "piece_macros.h"

#define KNIGHT_TOTAL_DIRECTIONS 8
#define KNIGHT_MAX_SCALE 1
const vector2_t KNIGHT_DIRECTIONS[] = {{-2, -1}, {-2, 1}, {-1, 2}, {1, 2}, {2, -1}, {2, 1}, {-1, -2}, {1, -2}};

piece_clone_fn knight_piece_clone;
piece_get_moves_fn knight_piece_get_moves;
piece_free_fn knight_piece_free;

DEFINE_PIECE_NEW_INTERNAL(knight, KNIGHT)
DEFINE_PIECE_CLONE_INTERNAL(knight)
DEFINE_PIECE_GET_MOVES_INTERNAL(_knight_get_moves, knight, KNIGHT)
DEFINE_PIECE_FREE_INTERNAL(knight)

DEFINE_PIECE_NEW(knight)
DEFINE_PIECE_CAST(knight, KNIGHT)
DEFINE_PIECE_CLONE(knight)
DEFINE_PIECE_GET_MOVES(knight)
DEFINE_PIECE_FREE(knight)

DEFINE_BOARD_IS_POSITION_GET_ATTACKED_BY_PIECE(knight, KNIGHT)
