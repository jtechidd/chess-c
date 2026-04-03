#include "move.h"
#include "defines.h"
#include "piece.h"
#include <string.h>

void move_init_move_piece(move_t *move, piece_t *piece,
                          vector2_t *position_to) {
  memset(move, 0, sizeof(move_t));

  move->flags = MOVE_FLAGS_MOVE_PIECE;
  move->piece_id = piece->id;
  move->position_from = piece->position;
  vector2_memcpy(&move->position_to, position_to);
}

static void move_with_take_piece(move_t *move, piece_t *take_piece) {
  move->flags |= MOVE_FLAGS_TAKE_PIECE;
  move->take_piece_id = take_piece->id;
}

void move_init_take_piece(move_t *move, piece_t *piece, vector2_t *position_to,
                          piece_t *take_piece) {
  move_init_move_piece(move, piece, position_to);
  move_with_take_piece(move, take_piece);
}

void move_init_castle(move_t *move, uint8_t castle_type) {
  memset(move, 0, sizeof(move_t));
  move->flags = MOVE_FLAGS_CASTLE;
  move->castle_type = castle_type;
}

void move_with_promotion(move_t *move, uint8_t promote_to) {
  move->flags |= MOVE_FLAGS_PROMOTE;
  move->promote_to = promote_to;
}

void move_memcpy(move_t *dest, move_t *src) {
  memcpy(dest, src, sizeof(move_t));
}
