#include "core/common.h"
#include "core/piece.h"

static CH_Error CH_Knight_CheckMove(CH_Piece *piece, CH_Chess *chess,
                                    CH_Move move, CH_Piece **takingPiece) {
  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_KNIGHT_METHODS = {
    .validateMove = CH_Knight_CheckMove,
};