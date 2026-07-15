#include <assert.h>
#include <stdio.h>

#include "core/chess.h"
#include "core/common.h"
#include "core/move.h"
#include "core/piece.h"
#include "core/piece_db.h"
#include "core/utils.h"
#include "core/vector2.h"

static void CH_Cell_InitWithPiece(CH_Cell *cell, CH_Piece *piece) {
  cell->pieceId = piece->id;
  cell->hasPiece = true;
}

static void CH_Board_PlacePiece(CH_Board *board, CH_Piece *piece) {
  CH_Cell_InitWithPiece(&board->table[piece->position.i][piece->position.j],
                        piece);
}

static void CH_Board_Clear(CH_Board *board) {
  memset(board, 0, sizeof(CH_Board));
}

static void CH_Chess_CreateAndPlacePiece(CH_Chess *chess, CH_Side side,
                                         CH_PieceType type, CH_Vector2 position,
                                         CH_PieceData data) {
  CH_Piece *piece;
  piece =
      CH_PieceDB_CreatePiece(&chess->pieceDb, side, type, position,
                             CH_PieceData_MakeRook(CH_ROOK_TYPE_QUEEN_SIDE));
  CH_Board_PlacePiece(&chess->board, piece);
}

void CH_Chess_InitStandard(struct CH_Chess *chess) {
  memset(chess, 0, sizeof(struct CH_Chess));
  chess->turn = CH_SIDE_WHITE;

  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_ROOK,
                               CH_Vector2_Make(0, 0),
                               CH_PieceData_MakeRook(CH_ROOK_TYPE_QUEEN_SIDE));
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_KNIGHT,
                               CH_Vector2_Make(0, 1), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_BISHOP,
                               CH_Vector2_Make(0, 2), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_QUEEN,
                               CH_Vector2_Make(0, 3), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_KING,
                               CH_Vector2_Make(0, 4), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_BISHOP,
                               CH_Vector2_Make(0, 5), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_KNIGHT,
                               CH_Vector2_Make(0, 6), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_ROOK,
                               CH_Vector2_Make(0, 7), CH_PieceData_MakeEmpty());

  for (uint8_t i = 0; i < 8; i++) {
    CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_BLACK, CH_PIECE_TYPE_PAWN,
                                 CH_Vector2_Make(1, i),
                                 CH_PieceData_MakeEmpty());
  }

  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_ROOK,
                               CH_Vector2_Make(7, 0),
                               CH_PieceData_MakeRook(CH_ROOK_TYPE_QUEEN_SIDE));
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_KNIGHT,
                               CH_Vector2_Make(7, 1), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_BISHOP,
                               CH_Vector2_Make(7, 2), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_QUEEN,
                               CH_Vector2_Make(7, 3), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_KING,
                               CH_Vector2_Make(7, 4), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_BISHOP,
                               CH_Vector2_Make(7, 5), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_KNIGHT,
                               CH_Vector2_Make(7, 6), CH_PieceData_MakeEmpty());
  CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_ROOK,
                               CH_Vector2_Make(7, 7), CH_PieceData_MakeEmpty());

  for (uint8_t i = 0; i < 8; i++) {
    CH_Chess_CreateAndPlacePiece(chess, CH_SIDE_WHITE, CH_PIECE_TYPE_PAWN,
                                 CH_Vector2_Make(6, i),
                                 CH_PieceData_MakeEmpty());
  }
}

static void CH_Chess_UpdateBoard(CH_Chess *chess) {
  CH_Board *board = &chess->board;
  CH_PieceDB *pieceDb = &chess->pieceDb;
  CH_Board_Clear(&chess->board);
  for (uint8_t i = 0; i < pieceDb->numPieces; i++) {
    CH_Piece *piece = CH_PieceDB_GetByIndex(pieceDb, i);
    if (!piece->isCaptured) {
      CH_Board_PlacePiece(board, piece);
    }
  }
}

CH_Piece *CH_Chess_GetPieceOnPosition(CH_Chess *chess, CH_Vector2 position) {
  if (!CH_IsPositionInBound(position)) {
    return NULL;
  }
  CH_Cell *cell = &chess->board.table[position.i][position.j];
  if (!cell->hasPiece) {
    return NULL;
  }
  assert(cell->pieceId != CH_EMPTY);
  return CH_PieceDB_GetById(&chess->pieceDb, cell->pieceId);
}

CH_Error CH_Chess_ApplyMove(CH_Chess *chess, CH_Move move) {
  CH_Piece *piece = CH_Chess_GetPieceOnPosition(chess, move.positionFrom);
  CH_Piece *takingPiece = NULL;

  // 1. Perform common move checking
  if (!(CH_IsPositionInBound(move.positionFrom) &&
        CH_IsPositionInBound(move.positionTo))) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (piece == NULL) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.pieceType != CH_EMPTY && piece->type != move.pieceType) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (!CH_Vector2_Equal(piece->position, move.positionFrom)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (piece->side != chess->turn) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.isTaking) {
    takingPiece = CH_Chess_GetPieceOnPosition(chess, move.positionTo);
    if (piece->type != CH_PIECE_TYPE_PAWN && !takingPiece) {
      return CH_ERR_ILLEGAL_MOVE;
    }
    if (takingPiece && takingPiece->side == piece->side) {
      return CH_ERR_ILLEGAL_MOVE;
    }
  } else if (CH_Chess_GetPieceOnPosition(chess, move.positionTo)) {
    return CH_ERR_ILLEGAL_MOVE;
  }
  if (move.promoteTo != CH_EMPTY && piece->type != CH_PIECE_TYPE_PAWN) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  // 2. Perform specific move checking by piece type
  if (CH_Piece_ValidateMove(piece, chess, move, &takingPiece) !=
      CH_ERR_SUCCESS) {
    return CH_ERR_ILLEGAL_MOVE;
  }

  // 3. Mutate pieces state
  piece->position = move.positionTo;
  piece->moveCount++;
  if (move.isTaking) {
    assert(takingPiece != NULL);
    takingPiece->isCaptured = true;
  }
  if (move.promoteTo != CH_EMPTY) {
    piece->type = move.promoteTo;
  }

  // 4. Update board
  CH_Chess_UpdateBoard(chess);

  // 5. Flip turn
  if (chess->turn == CH_SIDE_WHITE) {
    chess->turn = CH_SIDE_BLACK;
  } else {
    chess->turn = CH_SIDE_WHITE;
  }

  return CH_ERR_SUCCESS;
}

CH_Error CH_Chess_ApplyMoveLAN(CH_Chess *chess, const char *moveNotation) {
  CH_Move move;
  CH_Error error;

  error = CH_Move_ParseLAN(&move, moveNotation);
  if (error != CH_ERR_SUCCESS) {
    return error;
  }
  error = CH_Chess_ApplyMove(chess, move);
  if (error != CH_ERR_SUCCESS) {
    return error;
  }

  return CH_ERR_SUCCESS;
}

void CH_Chess_PrintBoard(CH_Chess *chess) {
  for (uint8_t i = 0; i < CH_BOARD_HEIGHT; i++) {
    for (uint8_t j = 0; j < CH_BOARD_WIDTH; j++) {
      CH_Piece *piece =
          CH_Chess_GetPieceOnPosition(chess, CH_Vector2_Make(i, j));
      if ((i + j) % 2 == 1) {
        printf("\033[40m");
      } else {
        printf("\033[0m");
      }
      if (piece == NULL) {
        putchar(' ');
      } else {
        if (piece->side == CH_SIDE_BLACK) {
          printf("\033[90m");
        }
        putchar(CH_PieceTypeToChar(piece->type));
      }
      printf("\033[0m");
    }
    putchar('\n');
  }
}