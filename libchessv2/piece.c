#include "piece.h"
#include "defines.h"

#include <string.h>

static const piece_methods_t *PIECE_METHODS[] = {
    [PIECE_TYPE_ROOK] = &PIECE_METHODS_ROOK,
    [PIECE_TYPE_KNIGHT] = &PIECE_METHODS_KNIGHT,
    [PIECE_TYPE_BISHOP] = &PIECE_METHODS_BISHOP,
    [PIECE_TYPE_QUEEN] = &PIECE_METHODS_QUEEN,
    [PIECE_TYPE_KING] = &PIECE_METHODS_KING,
    [PIECE_TYPE_PAWN] = &PIECE_METHODS_PAWN,
};

void piece_init(piece_t *piece, uint8_t type, uint8_t side, vector2_t *position,
                piece_data_t *data) {
  memset(piece, 0, sizeof(piece_t));

  piece->type = type;
  piece->side = side;
  piece->is_captured = 0;
  piece->move_count = 0;
  vector2_memcpy(&piece->position, position);
  piece_data_memcpy(&piece->data, data);
}

uint8_t piece_is_opposite(piece_t *piece_a, piece_t *piece_b) {
  return piece_a->side != piece_b->side;
}

void piece_memcpy(piece_t *dest, piece_t *src) {
  memcpy(dest, src, sizeof(piece_t));
}

void piece_data_init_empty(piece_data_t *piece_data) {
  memset(piece_data, 0, sizeof(piece_data_t));
}

void piece_data_memcpy(piece_data_t *dest, piece_data_t *src) {
  memcpy(dest, src, sizeof(piece_data_t));
}

void piece_fill_moves(chess_t *chess, piece_t *piece) {
  PIECE_METHODS[piece->type]->piece_fill_moves(chess, piece);
}