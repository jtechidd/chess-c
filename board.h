#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "cell.h"

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define TOTAL_PIECES 32

typedef struct Board {
    Cell cells[BOARD_HEIGHT][BOARD_WIDTH];
    Piece* pieces[TOTAL_PIECES];
} Board;

Board* Board_New();
Board* Board_Clone(Board*);
void Board_ClearCells(Board*);
void Board_UpdateCells(Board*);
void Board_RegisterPiecesNewGame(Board*);
void Board_RegisterPiecesFromBoard(Board*, Board*);
void Board_RegisterPiece(Board*, Piece*);
Piece* Board_GetPieceById(Board*, PieceId);
Piece* Board_GetPieceByPosition(Board*, Position);
void Board_Free(Board*);
void Board_Debug(Board*);

#endif