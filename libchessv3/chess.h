#ifndef CH_CHESS_H
#define CH_CHESS_H

#include "vector2.h"
#include "common.h"
#include "piece_db.h"
#include "move_db.h"
#include "move.h"

typedef struct {
  CH_PieceId pieceId;
  bool hasPiece;
} CH_Cell;

typedef struct {
  CH_Cell table[CH_BOARD_HEIGHT][CH_BOARD_WIDTH];
} CH_Board;

struct CH_Chess {
  CH_Side turn;
  CH_Board board;
  CH_MoveDB moveDb;
  CH_PieceDB pieceDb;
};

CH_Piece* CH_Chess_GetPieceOnPosition(CH_Chess *chess, CH_Vector2 position);
void CH_Chess_InitStandard(CH_Chess *chess);
CH_Error CH_Chess_ApplyMove(CH_Chess *chess, CH_Move move);

#endif