#ifndef CH_PIECE_DB_H
#define CH_PIECE_DB_H

#include "core/common.h"
#include "core/piece.h"

typedef struct {
  uint8_t numPieces;
  CH_PieceId cid;
  CH_Piece pieces[CH_MAX_PIECES];
} CH_PieceDB;

CH_Piece *CH_PieceDB_CreatePiece(CH_PieceDB *pieceDb, CH_Side side,
                                 CH_PieceType type, CH_Vector2 position,
                                 CH_PieceData data);
CH_Piece *CH_PieceDB_GetById(CH_PieceDB *pieceDb, CH_PieceId id);
CH_Piece *CH_PieceDB_GetByIndex(CH_PieceDB *pieceDB, uint8_t idx);
CH_Piece *CH_PieceDB_CreatePiece(CH_PieceDB *pieceDb, CH_Side side,
                                 CH_PieceType type, CH_Vector2 position,
                                 CH_PieceData data);
CH_Piece *CH_PieceDB_FindKingBySide(CH_PieceDB *pieceDb, CH_Side side);
CH_Piece *CH_PieceDB_FindRookBySideAndType(CH_PieceDB *pieceDb, CH_Side side,
                                           CH_RookType type);

#endif