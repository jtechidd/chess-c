
#include "core/piece_db.h"
#include "core/common.h"
#include "core/piece.h"

CH_Piece *CH_PieceDB_CreatePiece(CH_PieceDB *pieceDb, CH_Side side,
                                 CH_PieceType type, CH_Vector2 position,
                                 CH_PieceData data, const CH_PieceMethods *methods) {
  pieceDb->cid++;
  CH_Piece_Init(&pieceDb->pieces[pieceDb->numPieces], pieceDb->cid, side, type,
                position, data, methods);
  pieceDb->numPieces++;
  return &pieceDb->pieces[pieceDb->numPieces - 1];
}

CH_Piece *CH_PieceDB_GetById(CH_PieceDB *pieceDb, CH_PieceId id) {
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].id == id) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

CH_Piece *CH_PieceDB_GetByIndex(CH_PieceDB *pieceDb, uint8_t idx) {
  if (idx >= 0 && idx < pieceDb->numPieces) {
    return &pieceDb->pieces[idx];
  }
  return NULL;
}

CH_Piece *CH_PieceDB_FindKingBySide(CH_PieceDB *pieceDb, CH_Side side) {
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_KING) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

CH_Piece *CH_PieceDB_FindRookBySideAndType(CH_PieceDB *pieceDb, CH_Side side,
                                           CH_RookType type) {
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_ROOK &&
        pieceDb->pieces[i].data.rook.type == type) {
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}
