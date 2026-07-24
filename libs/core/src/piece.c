#include <assert.h>

#include "core/common.h"
#include "core/piece.h"

void CH_Piece_Init(CH_Piece *piece, CH_PieceId id, CH_Side side,
                   CH_PieceType type, CH_Vector2 position, CH_PieceData data,
                   const CH_PieceMethods *methods) {
  memset(piece, 0, sizeof(CH_Piece));

  piece->id = id;
  piece->side = side;
  piece->type = type;
  piece->position = position;
  piece->moveCount = 0;
  piece->isCaptured = false;
  piece->data = data;

  piece->methods = methods;
}

CH_Error CH_Piece_ValidateMove(CH_Piece *piece, CH_Chess *chess, CH_Move move,
                               CH_Piece **takingPiece) {
  assert(piece->methods && piece->methods->validateMove);
  return piece->methods->validateMove(piece, chess, move, takingPiece);
}

CH_PieceData CH_PieceData_MakeEmpty() { return (CH_PieceData){}; }

CH_PieceData CH_PieceData_MakeRook(CH_RookType type) {
  return (CH_PieceData){.rook = {.type = type}};
}