#include <assert.h>

#include "common.h"
#include "piece.h"

static const CH_PieceMethods *CH_PIECE_METHODS[] = {
    [CH_PIECE_TYPE_ROOK] = &CH_ROOK_METHODS,
    [CH_PIECE_TYPE_KNIGHT] = &CH_KNIGHT_METHODS,
    [CH_PIECE_TYPE_BISHOP] = &CH_BISHOP_METHODS,
    [CH_PIECE_TYPE_QUEEN] = &CH_QUEEN_METHODS,
    [CH_PIECE_TYPE_KING] = &CH_KING_METHODS,
    [CH_PIECE_TYPE_PAWN] = &CH_PAWN_METHODS,
};

void CH_Piece_Init(CH_Piece *piece, CH_PieceId id, CH_Side side,
                   CH_PieceType type, CH_Vector2 position, CH_PieceData data) {
  memset(piece, 0, sizeof(CH_Piece));

  piece->id = id;
  piece->side = side;
  piece->type = type;
  piece->position = position;
  piece->moveCount = 0;
  piece->isCaptured = false;
  piece->data = data;
}

const CH_PieceMethods *CH_Piece_GetMethods(CH_Piece *piece) {
  if (piece->type != CH_EMPTY && piece->type <= CH_NUM_PIECE_TYPES) {
    return CH_PIECE_METHODS[piece->type];
  }
  return NULL;
}

CH_Error CH_Piece_ValidateMove(CH_Piece *piece, CH_Chess *chess, CH_Move move, CH_Piece **takingPiece) {
  const CH_PieceMethods *methods = CH_Piece_GetMethods(piece);
  assert(methods != NULL);
  return methods->validateMove(piece, chess, move, takingPiece);
}

CH_PieceData CH_PieceData_MakeEmpty() { return (CH_PieceData){}; }

CH_PieceData CH_PieceData_MakeRook(CH_RookType type) {
  return (CH_PieceData){.rook = {.type = type}};
}