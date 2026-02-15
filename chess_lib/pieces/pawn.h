#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

typedef struct move_t move_t;

typedef struct pawn_t {
  piece_t piece;
  unsigned int can_get_en_passant : 1;
} pawn_t;

piece_t *pawn_piece_new(piece_id_t, side_t, vector2_t);
pawn_t *pawn_piece_cast(piece_t *);
void pawn_piece_flag_can_get_en_passant(piece_t *, move_t *);
void pawn_piece_promote(piece_t *, move_t *, board_t *);

board_is_position_get_attacked_by_piece_fn
    board_is_position_being_attacked_by_pawn;

#endif