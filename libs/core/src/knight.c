#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static bool CH_Knight_CheckDisplacement(CH_Vector2 disp) {
  if (disp.i != 0 && disp.j != 0 && abs(disp.i) + abs(disp.j) == 3)
    return true;
  return false;
}

static CH_Error CH_Knight_ValidateMove(CH_Piece *piece, CH_Chess *chess,
                                       CH_Move move, CH_Piece **takingPiece) {
  CH_Vector2 disp = CH_Vector2_Sub(move.positionTo, move.positionFrom);
  if (!CH_Knight_CheckDisplacement(disp)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_KNIGHT_METHODS = {
    .validateMove = CH_Knight_ValidateMove,
};