#include "core/common.h"
#include "core/move.h"
#include "core/piece.h"

static CH_Error CH_Bishop_CheckMove(CH_Piece *piece, CH_Chess *chess,
                                    CH_Move move, CH_Piece **takingPiece) {
  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_BISHOP_METHODS = {
    .validateMove = CH_Bishop_CheckMove,
};