#include "move.h"

#include <stdio.h>
#include <stdlib.h>

move_t *move_new() {
  move_t *move = (move_t *)malloc(sizeof(move_t));
  move->flags = 0;
  return move;
}

move_t *move_clone(move_t *move) {
  move_t *cloned_move = (move_t *)malloc(sizeof(move_t));

  cloned_move->flags = move->flags;

  cloned_move->piece_id = move->piece_id;
  cloned_move->position_to = move->position_to;
  cloned_move->take_piece_id = move->piece_id;
  cloned_move->promote_to = move->promote_to;
  cloned_move->castling_type = move->castling_type;
  return cloned_move;
}

move_t *move_new_moving_piece(piece_id_t piece_id, vector2_t position_to) {
  move_t *move = move_new();
  move->flags = MOVE_FLAGS_HAS_MOVING_PIECE;
  move->piece_id = piece_id;
  move->position_to = position_to;
  return move;
}

move_t *move_new_taking_piece(piece_id_t piece_id, vector2_t position_to,
                              piece_id_t take_piece_id) {
  move_t *move = move_new();
  move->flags = (MOVE_FLAGS_HAS_MOVING_PIECE | MOVE_FLAGS_HAS_TAKING_PIECE);
  move->piece_id = piece_id;
  move->position_to = position_to;
  move->take_piece_id = take_piece_id;
  return move;
}

move_t *move_new_castling(piece_id_t piece_id,
                          move_castling_type_t castling_type) {
  move_t *move = move_new();
  move->flags = MOVE_FLAGS_HAS_CASTLING;
  move->piece_id = piece_id;
  move->castling_type = castling_type;
  return move;
}

void move_with_promotion(move_t *move, piece_type_t promote_to) {
  move->flags |= MOVE_FLAGS_HAS_PROMOTION;
  move->promote_to = promote_to;
}

void move_debug(move_t *move) {
  if (move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) {
    printf("piece_id=%d position=(%d,%d) ", move->piece_id, move->position_to.i,
           move->position_to.j);
  }
  if (move->flags & MOVE_FLAGS_HAS_TAKING_PIECE) {
    printf("take_piece_id=%d", move->take_piece_id);
  }
  if (move->flags & MOVE_FLAGS_HAS_PROMOTION) {
    printf("promote_to=%d ", move->promote_to);
  }
  if (move->flags & MOVE_FLAGS_HAS_CASTLING) {
    printf("piece_id=%d castling_type=%d", move->piece_id, move->castling_type);
  }
  printf("\n");
}

void move_free(move_t *move) { free(move); }