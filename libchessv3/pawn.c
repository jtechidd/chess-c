#include "chess.h"
#include "common.h"
#include "piece.h"
#include "vector2.h"

static CH_Error CH_Pawn_ValidateMove(CH_Piece *piece, CH_Chess *chess,
                                  CH_Move move, CH_Piece **takingPiece) {
  CH_Vector2 up = CH_Vector2_Make(-1, 0);
  CH_Vector2 up2 = CH_Vector2_Make(-2, 0);
  CH_Vector2 left = CH_Vector2_Make(0, -1);
  CH_Vector2 right = CH_Vector2_Make(0, 1);
  CH_Vector2 upLeft = CH_Vector2_Make(-1, -1);
  CH_Vector2 upRight = CH_Vector2_Make(-1, 1);

  if (piece->side == CH_SIDE_BLACK) {
    up = CH_Vector2_FlipV(up);
    up2 = CH_Vector2_FlipV(up2);
    upLeft = CH_Vector2_FlipV(upLeft);
    upRight = CH_Vector2_FlipV(upRight);
  }

  CH_Vector2 disp = CH_Vector2_Sub(move.positionTo, move.positionFrom);
  CH_Vector2 posFromUp = CH_Vector2_Add(move.positionFrom, up);
  CH_Vector2 posFromUp2 = CH_Vector2_Add(move.positionFrom, up2);
  CH_Vector2 posFromLeft = CH_Vector2_Add(move.positionFrom, left);
  CH_Vector2 posFromRight = CH_Vector2_Add(move.positionFrom, right);

  CH_Piece *enPassantPiece;

  if (move.isTaking) {
    if (!CH_Vector2_Equal(disp, upLeft) && !CH_Vector2_Equal(disp, upRight)) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    if (*takingPiece == NULL) {
      if (CH_Vector2_Equal(disp, upLeft)) {
        enPassantPiece = CH_Chess_GetPieceOnPosition(chess, posFromLeft);
      } else if (CH_Vector2_Equal(disp, upRight)) {
        enPassantPiece = CH_Chess_GetPieceOnPosition(chess, posFromLeft);
      }
      if (!(enPassantPiece && enPassantPiece->type == CH_PIECE_TYPE_PAWN &&
            enPassantPiece->side != piece->side &&
            enPassantPiece->moveCount == 1)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      *takingPiece = enPassantPiece;
    }
  } else {
    disp = CH_Vector2_Sub(move.positionTo, move.positionFrom);
    if (CH_Vector2_Equal(disp, up2)) {
      if (piece->moveCount > 0) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (CH_Chess_GetPieceOnPosition(chess, posFromUp) ||
          CH_Chess_GetPieceOnPosition(chess, posFromUp2)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    } else if (CH_Vector2_Equal(disp, up)) {
      if (CH_Chess_GetPieceOnPosition(chess, posFromUp)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    } else {
      return CH_ERR_ILLEGAL_MOVE;
    }
  }

  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_PAWN_METHODS = {
    .validateMove = CH_Pawn_ValidateMove,
};