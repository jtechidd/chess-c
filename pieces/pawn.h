#ifndef PAWN_H
#define PAWN_H

#include "piece.h"

typedef struct move_t move_t;

typedef struct pawn_t {
  piece_t piece;
  unsigned int can_get_en_passant : 1;
} pawn_t;

#define WUR __attribute__((warn_unused_result()))

WUR piece_new_fn pawn_piece_new;
WUR int pawn_piece_cast(pawn_t **, piece_t *);
WUR int pawn_piece_flag_can_get_en_passant(piece_t *, move_t *);
WUR int pawn_piece_promote(piece_t *, move_t *, board_t *);

WUR board_is_position_get_attacked_by_piece_fn board_is_position_get_attacked_by_pawn;

#undef WUR

#endif