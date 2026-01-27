#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

typedef struct pawn_t {
  piece_t piece;
  unsigned int can_get_en_passant : 1;
} pawn_t;

pawn_t *pawn_new(piece_id_t, side_t, vector2_t);
pawn_t *pawn_clone(pawn_t *);
pawn_t *pawn_cast(piece_t *);
move_array_t *pawn_get_moves(piece_t *, board_t *);
void pawn_flag_can_get_en_passant(piece_t *, move_t *);
void pawn_promote(piece_t *, move_t *, board_t *);
void pawn_free(piece_t *);

uint8_t board_is_position_get_attacked_by_pawn(board_t *, side_t, vector2_t);

#endif