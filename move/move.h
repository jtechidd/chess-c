#ifndef MOVE_H
#define MOVE_H

#include "../enums.h"
#include "../vector2.h"

typedef struct Move {
    MoveFlags flags;

    PieceId piece_id;
    Vector2 position_to;

    PieceId take_piece_id;

    PieceType promote_to;

    MoveCastlingType castling_type;
} Move;

Move* move_new();
Move* move_clone(Move*);
Move* move_new_moving_piece(PieceId, Vector2);
Move* move_new_taking_piece(PieceId, Vector2, PieceId);
Move* move_new_castling(MoveCastlingType);
void move_with_moving_piece(Move*, PieceId, Vector2);
void move_with_taking_piece(Move*, PieceId);
void move_with_promotion(Move*, PieceType);
void move_debug(Move*);
void move_free(Move*);

#endif