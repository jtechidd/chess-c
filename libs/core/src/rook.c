#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static bool CH_Rook_CheckDisplacement(CH_Vector2 disp) {
  return disp.i == 0 != disp.j == 0;
}

static CH_Error CH_Rook_ValidateMove(CH_Piece *piece, CH_Chess *chess,
                                     CH_Move move, CH_Piece **takingPiece) {
  CH_Vector2 disp = CH_Vector2_Sub(move.positionTo, move.positionFrom);
  if (!CH_Rook_CheckDisplacement(disp)) {
    return false;
  }
  CH_Vector2 dir = CH_Vector2_Make(disp.i == 0 ? 0 : (disp.i / abs(disp.i)),
                                   disp.j == 0 ? 0 : (disp.j / abs(disp.j)));
  CH_Vector2 dest;
  for (uint8_t k = 1; k < 8; k++) {
    dest = CH_Vector2_Add(move.positionFrom, CH_Vector2_ScalMult(dir, k));
    if (CH_Vector2_Equal(dest, move.positionTo)) {
      break;
    }
    if (CH_Chess_GetPieceOnPosition(chess, dest)) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  }

  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_ROOK_METHODS = {
    .validateMove = CH_Rook_ValidateMove,
};
