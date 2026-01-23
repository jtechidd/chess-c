#include "board.h"

#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include "pieces/bishop.h"
#include "pieces/king.h"
#include "pieces/knight.h"
#include "pieces/pawn.h"
#include "pieces/queen.h"
#include "pieces/rook.h"

Board* Board_New() {
    Board* board = (Board*)malloc(sizeof(Board));

    Board_RegisterPiecesNewGame(board);
    Board_UpdateCells(board);

    return board;
}

Board* Board_Clone(Board* board_src) {
    Board* board = (Board*)malloc(sizeof(Board));

    Board_RegisterPiecesFromBoard(board, board_src);
    Board_UpdateCells(board);

    return board;
}

void Board_ClearCells(Board* b) {
    memset(b->cells, 0, sizeof(b->cells));
}

void Board_UpdateCells(Board* board) {
    Board_ClearCells(board);
    for (PieceId piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
        Piece* piece = Board_GetPieceById(board, piece_id);
        board->cells[piece->position.i][piece->position.j] = Cell_Make(1, piece->id);
    }
}

void Board_RegisterPiecesNewGame(Board* board) {
    Board_RegisterPiece(board, (Piece*)Rook_New(PieceId_BlackRook1, Side_Black, Position_Make(0, 0)));
    Board_RegisterPiece(board, (Piece*)Knight_New(PieceId_BlackKnight1, Side_Black, Position_Make(0, 1)));
    Board_RegisterPiece(board, (Piece*)Bishop_New(PieceId_BlackBishop1, Side_Black, Position_Make(0, 2)));
    Board_RegisterPiece(board, (Piece*)Queen_New(PieceId_BlackQueen, Side_Black, Position_Make(0, 3)));
    Board_RegisterPiece(board, (Piece*)King_New(PieceId_BlackKing, Side_Black, Position_Make(0, 4)));
    Board_RegisterPiece(board, (Piece*)Bishop_New(PieceId_BlackBishop2, Side_Black, Position_Make(0, 5)));
    Board_RegisterPiece(board, (Piece*)Knight_New(PieceId_BlackKnight2, Side_Black, Position_Make(0, 6)));
    Board_RegisterPiece(board, (Piece*)Rook_New(PieceId_BlackRook2, Side_Black, Position_Make(0, 7)));
    for (PieceId j = 0; j < 8; j++) {
        Board_RegisterPiece(board, (Piece*)Pawn_New(PieceId_BlackPawn1 + j, Side_Black, Position_Make(1, j)));
    }

    Board_RegisterPiece(board, (Piece*)Rook_New(PieceId_WhiteRook1, Side_White, Position_Make(7, 0)));
    Board_RegisterPiece(board, (Piece*)Knight_New(PieceId_WhiteKnight1, Side_White, Position_Make(7, 1)));
    Board_RegisterPiece(board, (Piece*)Bishop_New(PieceId_WhiteBishop1, Side_White, Position_Make(7, 2)));
    Board_RegisterPiece(board, (Piece*)Queen_New(PieceId_WhiteQueen, Side_White, Position_Make(7, 3)));
    Board_RegisterPiece(board, (Piece*)King_New(PieceId_WhiteKing, Side_White, Position_Make(7, 4)));
    Board_RegisterPiece(board, (Piece*)Bishop_New(PieceId_WhiteBishop2, Side_White, Position_Make(7, 5)));
    Board_RegisterPiece(board, (Piece*)Knight_New(PieceId_WhiteKnight2, Side_White, Position_Make(7, 6)));
    Board_RegisterPiece(board, (Piece*)Rook_New(PieceId_WhiteRook2, Side_White, Position_Make(7, 7)));
    for (PieceId j = 0; j < 8; j++) {
        Board_RegisterPiece(board, (Piece*)Pawn_New(PieceId_WhitePawn1 + j, Side_White, Position_Make(6, j)));
    }
}

void Board_RegisterPiecesFromBoard(Board* board_dest, Board* board_src) {
    for (PieceId piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
        Piece* piece = Board_GetPieceById(board_src, piece_id);
        Piece* new_piece;
        switch (piece->type) {
            case PieceType_Rook:
                new_piece = (Piece*)Rook_Clone((Rook*)piece);
                break;
            case PieceType_Knight:
                new_piece = (Piece*)Knight_Clone((Knight*)piece);
                break;
            case PieceType_Bishop:
                new_piece = (Piece*)Bishop_Clone((Bishop*)piece);
                break;
            case PieceType_Queen:
                new_piece = (Piece*)Queen_Clone((Queen*)piece);
                break;
            case PieceType_King:
                new_piece = (Piece*)King_Clone((King*)piece);
                break;
            case PieceType_Pawn:
                new_piece = (Piece*)Pawn_Clone((Pawn*)piece);
                break;
        }
        Board_RegisterPiece(board_dest, new_piece);
    }
}

void Board_RegisterPiece(Board* board, Piece* piece) {
    board->pieces[piece->id] = piece;
}

Piece* Board_GetPieceById(Board* b, PieceId id) { return b->pieces[id]; }

Piece* Board_GetPieceByPosition(Board* b, Position pos) {
    if (Position_OutOfBound(&pos)) {
        return NULL;
    }
    Cell cell = b->cells[pos.i][pos.j];
    if (!cell.has_piece) {
        return NULL;
    }
    PieceId id = cell.piece_id;
    if (!PieceId_IsValid(id)) {
        return NULL;
    }
    return b->pieces[id];
}

MoveArray* Board_GetMoves(Board* board, Side side) {
    PieceId piece_id_start;
    PieceId piece_id_end;
    if (side == Side_White) {
        piece_id_start = PieceId_BlackRook1;
        piece_id_end = PieceId_BlackPawn8;
    } else if (side == Side_Black) {
        piece_id_start = PieceId_WhiteRook1;
        piece_id_end = PieceId_WhitePawn8;
    }

    MoveArray* all_moves = MoveArray_New();
    for (PieceId id = piece_id_start; id <= piece_id_end; id++) {
        Piece* piece = Board_GetPieceById(board, id);

        MoveArray* positional_moves = piece->Piece_GetPositionalMoves(board, piece);

        for (size_t i = 0; i < positional_moves->length; i++) {
            Move* move = MoveArray_GetIndex(positional_moves, i);
            Board* cloned_board = Board_Clone(board);
            Board_MakeMove(cloned_board, move);
            if (Board_IsKingInDanger(cloned_board, side)) {
                Move_Free(move);
            } else {
                MoveArray_Add(all_moves, move);
            }
            Board_Free(cloned_board);
        }

        MoveArray_Free(positional_moves);
    }

    return all_moves;
}

void Board_MakeMove(Board* board, Move* move) {

}

uint8_t Board_IsKingInDanger(Board* board, Side side) {
    // TODO: Check if king is checked from opposite side pieces
}

void Board_Free(Board* board) {
    for (PieceId id = 0; id < TOTAL_PIECES; id++) {
        Piece* piece = Board_GetPieceById(board, id);
        piece->Piece_Free(piece);
    }
    free(board);
}

void Board_Debug(Board* board) {
    for (size_t i = 0; i < BOARD_HEIGHT; i++) {
        for (size_t j = 0; j < BOARD_WIDTH; j++) {
            Piece* piece = Board_GetPieceByPosition(board, Position_Make(i, j));
            if (!piece) {
                printf(".");
                continue;
            }
            switch (piece->type) {
                case PieceType_Rook:
                    printf("r");
                    break;
                case PieceType_Knight:
                    printf("k");
                    break;
                case PieceType_Bishop:
                    printf("b");
                    break;
                case PieceType_Queen:
                    printf("q");
                    break;
                case PieceType_King:
                    printf("K");
                    break;
                case PieceType_Pawn:
                    printf("p");
                    break;
            }
        }
        printf("\n");
    }
}