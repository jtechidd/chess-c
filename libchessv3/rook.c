#include "common.h"
#include "piece.h"

static CH_Error CH_Rook_CheckMove(CH_Piece *piece, CH_Chess *chess,
                                  CH_Move move, CH_Piece **takingPiece) {
  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_ROOK_METHODS = {
    .validateMove = CH_Rook_CheckMove,
};