#ifndef CH_UTILS_H
#define CH_UTILS_H

#include "core/common.h"
#include "core/vector2.h"

bool CH_IsValidFile(char file);
bool CH_IsValidRank(char rank);
int8_t CH_RankToVertPos(char rank);
int8_t CH_FileToHorizPos(char file);
CH_PieceType CH_CharToPieceType(char pieceChar);
char CH_PieceTypeToChar(CH_PieceType type);
bool CH_IsPositionInBound(CH_Vector2 pos);

#endif