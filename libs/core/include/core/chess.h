#ifndef CH_CHESS_H
#define CH_CHESS_H

#include "core/common.h"
#include "core/move.h"
#include "core/piece_db.h"
#include "core/vector2.h"

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
  CH_PieceDB pieceDb;
};

CH_Piece *CH_Chess_GetPieceOnPosition(CH_Chess *chess, CH_Vector2 position);
void CH_Chess_InitStandard(CH_Chess *chess);
CH_Error CH_Chess_ApplyMove(CH_Chess *chess, CH_Move move);
CH_Error CH_Chess_ApplyMoveLAN(CH_Chess *chess, const char *move);
void CH_Chess_PrintBoard(CH_Chess *chess);

#endif