#ifndef CH_PIECE_H
#define CH_PIECE_H

#include "core/common.h"
#include "core/move.h"
#include "core/vector2.h"

typedef union {
  struct {
    CH_RookType type;
  } rook;
} CH_PieceData;

typedef struct CH_PieceMethods CH_PieceMethods;

typedef struct {
  CH_PieceId id;
  CH_Side side;
  CH_PieceType type;
  CH_Vector2 position;
  uint8_t moveCount;
  bool isCaptured;
  CH_PieceData data;
  const CH_PieceMethods *methods;
} CH_Piece;

struct CH_PieceMethods {
  CH_Error (*validateMove)(CH_Piece *piece, CH_Chess *chess, CH_Move move,
                           CH_Piece **takingPiece);
};

extern const CH_PieceMethods CH_ROOK_METHODS;
extern const CH_PieceMethods CH_KNIGHT_METHODS;
extern const CH_PieceMethods CH_BISHOP_METHODS;
extern const CH_PieceMethods CH_QUEEN_METHODS;
extern const CH_PieceMethods CH_KING_METHODS;
extern const CH_PieceMethods CH_PAWN_METHODS;

void CH_Piece_Init(CH_Piece *piece, CH_PieceId id, CH_Side side,
                   CH_PieceType type, CH_Vector2 position, CH_PieceData data,
                   const CH_PieceMethods *methods);
CH_Error CH_Piece_ValidateMove(CH_Piece *piece, CH_Chess *chess, CH_Move move,
                               CH_Piece **takingPiece);
CH_PieceData CH_PieceData_MakeEmpty();
CH_PieceData CH_PieceData_MakeRook(CH_RookType type);

#endif