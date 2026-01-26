#ifndef MOVE_H
#define MOVE_H

#include "../enums.h"
#include "../vector2.h"

typedef struct move_t {
    move_flags_t flags;

    piece_id_t piece_id;
    vector2_t position_to;

    piece_id_t take_piece_id;

    piece_type_t promote_to;

    move_castling_type_t castling_type;
} move_t;

move_t* move_new();
move_t* move_clone(move_t*);
move_t* move_new_moving_piece(piece_id_t, vector2_t);
move_t* move_new_taking_piece(piece_id_t, vector2_t, piece_id_t);
move_t* move_new_castling(move_castling_type_t);
void move_with_moving_piece(move_t*, piece_id_t, vector2_t);
void move_with_taking_piece(move_t*, piece_id_t);
void move_with_promotion(move_t*, piece_type_t);
void move_debug(move_t*);
void move_free(move_t*);

#endif