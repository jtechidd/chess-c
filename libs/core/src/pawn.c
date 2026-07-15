#include "core/chess.h"
#include "core/common.h"
#include "core/piece.h"
#include "core/vector2.h"

static bool CH_IsEnPassantable(CH_Piece *pawn, CH_Piece *enPassantPiece) {
  if (!enPassantPiece)
    return false;
  if (enPassantPiece->type != CH_PIECE_TYPE_PAWN)
    return false;
  if (enPassantPiece->side == pawn->side)
    return false;
  if (enPassantPiece->moveCount != 1)
    return false;
  if (enPassantPiece->side == CH_SIDE_WHITE && enPassantPiece->position.i != 4)
    return false;
  if (enPassantPiece->side == CH_SIDE_BLACK && enPassantPiece->position.i != 3)
    return false;
  return true;
}

static bool CH_IsValidPromoteTo(CH_PieceType type) {
  return type == CH_PIECE_TYPE_QUEEN || type == CH_PIECE_TYPE_ROOK ||
         type == CH_PIECE_TYPE_BISHOP || type == CH_PIECE_TYPE_KNIGHT;
}

static bool CH_IsPositionPromotable(CH_Vector2 pos, CH_Side side) {
  return side == CH_SIDE_WHITE && pos.i == 0 ||
         side == CH_SIDE_BLACK && pos.i == CH_BOARD_HEIGHT - 1;
}

static CH_Error CH_Pawn_ValidateMove(CH_Piece *pawn, CH_Chess *chess,
                                     CH_Move move, CH_Piece **takingPiece) {
  CH_Vector2 up = CH_Vector2_Make(-1, 0);
  CH_Vector2 up2 = CH_Vector2_Make(-2, 0);
  CH_Vector2 left = CH_Vector2_Make(0, -1);
  CH_Vector2 right = CH_Vector2_Make(0, 1);
  CH_Vector2 upLeft = CH_Vector2_Make(-1, -1);
  CH_Vector2 upRight = CH_Vector2_Make(-1, 1);

  if (pawn->side == CH_SIDE_BLACK) {
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

  CH_Piece *enPassantPiece = NULL;

  if (move.isTaking) {
    if (*takingPiece == NULL) {
      if (CH_Vector2_Equal(disp, upLeft)) {
        enPassantPiece = CH_Chess_GetPieceOnPosition(chess, posFromLeft);
      } else if (CH_Vector2_Equal(disp, upRight)) {
        enPassantPiece = CH_Chess_GetPieceOnPosition(chess, posFromRight);
      } else {
        return CH_ERR_ILLEGAL_MOVE;
      }
      if (!CH_IsEnPassantable(pawn, enPassantPiece)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
      *takingPiece = enPassantPiece;
    } else {
      if (!CH_Vector2_Equal(disp, upLeft) && !CH_Vector2_Equal(disp, upRight)) {
        return CH_ERR_ILLEGAL_MOVE;
      }
    }
  } else {
    disp = CH_Vector2_Sub(move.positionTo, move.positionFrom);
    if (CH_Vector2_Equal(disp, up2)) {
      if (pawn->moveCount > 0) {
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

  if (CH_IsPositionPromotable(move.positionTo, pawn->side)) {
    if (!CH_IsValidPromoteTo(move.promoteTo)) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  } else if (move.promoteTo != CH_EMPTY) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  return CH_ERR_SUCCESS;
}

const CH_PieceMethods CH_PAWN_METHODS = {
    .validateMove = CH_Pawn_ValidateMove,
};