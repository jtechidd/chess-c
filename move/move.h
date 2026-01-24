#ifndef MOVE_H
#define MOVE_H

#include "../enums.h"
#include "../position.h"

typedef struct Move {
    MoveFlags flags;

    PieceId piece_id;
    Position position;

    PieceId take_piece_id;

    PieceType promote_to;

    MoveCastlingType castling_type;
} Move;

Move* Move_New();
Move* Move_Clone(Move*);
Move* Move_NewMovingPiece(PieceId, Position);
Move* Move_NewTakingPiece(PieceId, Position, PieceId);
Move* Move_NewCastling(MoveCastlingType);
void Move_WithMovingPiece(Move*, PieceId, Position);
void Move_WithTakingPiece(Move*, PieceId);
void Move_WithPromotion(Move*, PieceType);
void Move_Free(Move*);

#endif