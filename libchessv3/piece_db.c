
#include "piece_db.h"

CH_Piece *CH_PieceDB_CreatePiece(CH_PieceDB *pieceDb, CH_Side side,
                                 CH_PieceType type, CH_Vector2 position,
                                 CH_PieceData data) {
  pieceDb->cid++;
  CH_Piece_Init(&pieceDb->pieces[pieceDb->numPieces], pieceDb->cid, side, type,
                position, data);
  pieceDb->numPieces++;
  return &pieceDb->pieces[pieceDb->numPieces - 1];
}

CH_Piece *CH_PieceDB_GetById(CH_PieceDB *pieceDb, CH_PieceId id) {
  if (id <= CH_MAX_PIECES && pieceDb->idToPiece[id] != NULL) {
    return pieceDb->idToPiece[id];
  }
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].id == id) {
      pieceDb->idToPiece[id] = &pieceDb->pieces[i];
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

CH_Piece *CH_PieceDB_GetByIndex(CH_PieceDB* pieceDb, uint8_t idx) {
  if(idx >= 0 && idx < pieceDb->numPieces) {
    return &pieceDb->pieces[idx];
  }
  return NULL;
}

CH_Piece *CH_PieceDB_FindKingBySide(CH_PieceDB *pieceDb, CH_Side side) {
  if (side <= CH_NUM_SIDES && pieceDb->sideToKing[side] != NULL) {
    return pieceDb->sideToKing[side];
  }
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_KING) {
      pieceDb->sideToKing[side] = &pieceDb->pieces[i];
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}

CH_Piece *CH_PieceDB_FindRookBySideAndType(CH_PieceDB *pieceDb, CH_Side side,
                                           CH_RookType type) {
  if (side <= CH_NUM_SIDES && type <= CH_NUM_ROOK_TYPES &&
      pieceDb->sideAndTypeToRook[side][type] != NULL) {
    return pieceDb->sideAndTypeToRook[side][type];
  }
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    if (pieceDb->pieces[i].side == side &&
        pieceDb->pieces[i].type == CH_PIECE_TYPE_ROOK &&
        pieceDb->pieces[i].data.rook.type == type) {
      pieceDb->sideToKing[side] = &pieceDb->pieces[i];
      return &pieceDb->pieces[i];
    }
  }
  return NULL;
}