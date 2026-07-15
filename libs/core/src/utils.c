#include "core/utils.h"
#include "core/common.h"

bool CH_IsValidFile(char file) {
  return isalpha(file) && file >= 'a' && file <= 'h';
}

bool CH_IsValidRank(char rank) {
  return isdigit(rank) && rank >= '1' && rank <= '8';
}

int8_t CH_RankToVertPos(char rank) {
  if (!CH_IsValidRank(rank)) {
    return -1;
  }
  return CH_BOARD_HEIGHT - (rank - '0');
}

int8_t CH_FileToHorizPos(char file) {
  if (!CH_IsValidFile(file)) {
    return -1;
  }
  return file - 'a';
}

CH_PieceType CH_CharToPieceType(char pieceChar) {
  switch (pieceChar) {
  case 'R':
    return CH_PIECE_TYPE_ROOK;
  case 'N':
    return CH_PIECE_TYPE_KNIGHT;
  case 'B':
    return CH_PIECE_TYPE_BISHOP;
  case 'Q':
    return CH_PIECE_TYPE_QUEEN;
  case 'K':
    return CH_PIECE_TYPE_KING;
  case 'P':
    return CH_PIECE_TYPE_PAWN;
  }
  return CH_EMPTY;
}

char CH_PieceTypeToChar(CH_PieceType type) {
  switch (type) {
  case CH_PIECE_TYPE_ROOK:
    return 'R';
  case CH_PIECE_TYPE_KNIGHT:
    return 'N';
  case CH_PIECE_TYPE_BISHOP:
    return 'B';
  case CH_PIECE_TYPE_QUEEN:
    return 'Q';
  case CH_PIECE_TYPE_KING:
    return 'K';
  case CH_PIECE_TYPE_PAWN:
    return 'P';
  }
  return CH_EMPTY;
}

bool CH_IsPositionInBound(CH_Vector2 pos) {
  return pos.i >= 0 && pos.i < CH_BOARD_HEIGHT && pos.j >= 0 &&
         pos.j < CH_BOARD_WIDTH;
}
