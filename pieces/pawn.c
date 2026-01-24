#include "pawn.h"

#include <stdlib.h>

#include "../board.h"
#include "../move/move.h"

void pawn_add_move_up(Board*, Pawn*, MoveArray*);
void pawn_add_move_double_up(Board*, Pawn*, MoveArray*);
void pawn_add_move_take_upper_left(Board*, Pawn*, MoveArray*);
void pawn_add_move_take_upper_right(Board*, Pawn*, MoveArray*);
void pawn_add_move_en_passant_left(Board*, Pawn*, MoveArray*);
void pawn_add_move_en_passant_right(Board*, Pawn*, MoveArray*);
void pawn_add_move(Pawn*, MoveArray*, Move*);
void pawn_add_promotion_moves(MoveArray*, Move*);

Pawn* Pawn_New(PieceId piece_id, Side side, Position position) {
    Pawn* piece = (Pawn*)malloc(sizeof(Pawn));

    // Set piece fields
    piece->piece.id = piece_id;
    piece->piece.side = side;
    piece->piece.type = PieceType_Pawn;
    piece->piece.position = position;
    piece->piece.is_captured = 0;

    // Set functions
    piece->piece.Piece_GetPositionalMoves = Pawn_GetPositionalMoves;
    piece->piece.Piece_Free = Pawn_Free;

    // Set fields
    piece->has_been_moved = 0;
    piece->first_time_moved = 0;

    return piece;
}

Pawn* Pawn_Clone(Pawn* p_src) {
    Pawn* p = Pawn_New(p_src->piece.id, p_src->piece.side, p_src->piece.position);

    // Set piece fields
    p->piece.is_captured = p_src->piece.is_captured;

    // Set fields
    p->has_been_moved = p_src->has_been_moved;

    return p;
}

MoveArray* Pawn_GetPositionalMoves(Board* board, Piece* piece) {
    Pawn* pawn = (Pawn*)piece;

    MoveArray* move_array = MoveArray_New();

    pawn_add_move_up(board, pawn, move_array);
    pawn_add_move_double_up(board, pawn, move_array);
    pawn_add_move_take_upper_left(board, pawn, move_array);
    pawn_add_move_take_upper_right(board, pawn, move_array);
    pawn_add_move_en_passant_left(board, pawn, move_array);
    pawn_add_move_en_passant_right(board, pawn, move_array);

    return move_array;
}

void pawn_add_move_up(Board* board, Pawn* pawn, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    if (side == Side_White) {
        position_to = Position_Make(position.i - 1, position.j);
    } else if (side == Side_Black) {
        position_to = Position_Make(position.i + 1, position.j);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (Board_HasPieceOnPosition(board, position_to)) {
        return;
    }

    Move* move = Move_NewMovingPiece(pawn->piece.id, position_to);
    pawn_add_move(pawn, move_array, move);
}

void pawn_add_move_double_up(Board* board, Pawn* pawn, MoveArray* move_array) {
    if (pawn->has_been_moved) {
        return;
    }

    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    if (side == Side_White) {
        position_to = Position_Make(position.i - 2, position.j);
    } else if (side == Side_Black) {
        position_to = Position_Make(position.i + 2, position.j);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (Board_HasPieceOnPosition(board, position_to)) {
        return;
    }

    Move* move = Move_NewMovingPiece(pawn->piece.id, position_to);
    pawn_add_move(pawn, move_array, move);
}

void pawn_add_move_take_upper_left(Board* board, Pawn* pawn, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    if (side = Side_White) {
        position_to = Position_Make(position.i - 1, position.j - 1);
    } else if (side = Side_Black) {
        position_to = Position_Make(position.i + 1, position.j + 1);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (!Board_CanTakePosition(board, &pawn->piece, position_to)) {
        return;
    }

    Piece* take_piece = Board_GetPieceByPosition(board, position_to);
    Move* move = Move_NewTakingPiece(pawn->piece.id, position_to, take_piece->id);
    pawn_add_move(pawn, move_array, move);
}

void pawn_add_move_take_upper_right(Board* board, Pawn* pawn, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    if (side = Side_White) {
        position_to = Position_Make(position.i - 1, position.j + 1);
    } else if (side = Side_Black) {
        position_to = Position_Make(position.i + 1, position.j - 1);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (!Board_CanTakePosition(board, &pawn->piece, position_to)) {
        return;
    }

    Piece* take_piece = Board_GetPieceByPosition(board, position_to);
    Move* move = Move_NewTakingPiece(pawn->piece.id, position_to, take_piece->id);
    pawn_add_move(pawn, move_array, move);
}

void pawn_add_move_en_passant_left(Board* board, Pawn* pawn, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    Position position_side_pawn;
    if (side == Side_White) {
        position_to = Position_Make(position.i - 1, position.j - 1);
        position_side_pawn = Position_Make(position.i, position.j - 1);
    } else if (side = Side_Black) {
        position_to = Position_Make(position.i + 1, position.j + 1);
        position_side_pawn = Position_Make(position.i, position.j + 1);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (!Board_IsPositionInBoundary(position_side_pawn)) {
        return;
    }

    Piece* piece = Board_GetPieceByPosition(board, position_side_pawn);
    if (!(piece && piece->type == PieceType_Pawn && Piece_IsOpposite(&pawn->piece, piece))) {
        return;
    }
    Pawn* side_pawn = (Pawn*)piece;
    if (!side_pawn->first_time_moved) {
        return;
    }

    Move* move = Move_NewTakingPiece(pawn->piece.id, position_to, side_pawn->piece.id);
    pawn_add_move(pawn, move_array, move);
}

void pawn_add_move_en_passant_right(Board* board, Pawn* pawn, MoveArray* move_array) {
    Side side = pawn->piece.side;
    Position position = pawn->piece.position;

    Position position_to;
    Position position_side_pawn;
    if (side == Side_White) {
        position_to = Position_Make(position.i - 1, position.j + 1);
        position_side_pawn = Position_Make(position.i, position.j + 1);
    } else if (side = Side_Black) {
        position_to = Position_Make(position.i + 1, position.j - 1);
        position_side_pawn = Position_Make(position.i, position.j - 1);
    }

    if (!Board_IsPositionInBoundary(position_to)) {
        return;
    }
    if (!Board_IsPositionInBoundary(position_side_pawn)) {
        return;
    }

    Piece* piece = Board_GetPieceByPosition(board, position_side_pawn);
    if (!(piece && piece->type == PieceType_Pawn && Piece_IsOpposite(&pawn->piece, piece))) {
        return;
    }
    Pawn* side_pawn = (Pawn*)piece;
    if (!side_pawn->first_time_moved) {
        return;
    }

    Move* move = Move_NewTakingPiece(pawn->piece.id, position_to, side_pawn->piece.id);
    pawn_add_move(pawn, move_array, move);
}

// Relaying to add single move or to add promotion moves
void pawn_add_move(Pawn* pawn, MoveArray* move_array, Move* move) {
    Side side = pawn->piece.side;

    uint8_t can_be_promoted = 0;
    if (side == Side_White) {
        can_be_promoted = Board_IsPositionTop(move->position);
    } else if (side == Side_Black) {
        can_be_promoted = Board_IsPositionBottom(move->position);
    }

    if (can_be_promoted) {
        pawn_add_promotion_moves(move_array, move);
    } else {
        MoveArray_Add(move_array, move);
    }
}

// Clone move and configured with promotable piece, then add to array
void pawn_add_promotion_moves(MoveArray* move_array, Move* move) {
    Move* cloned_move = Move_Clone(move);
    Move_WithPromotion(cloned_move, PieceType_Queen);
    MoveArray_Add(move_array, cloned_move);

    cloned_move = Move_Clone(move);
    Move_WithPromotion(cloned_move, PieceType_Rook);
    MoveArray_Add(move_array, cloned_move);

    cloned_move = Move_Clone(move);
    Move_WithPromotion(cloned_move, PieceType_Bishop);
    MoveArray_Add(move_array, cloned_move);

    cloned_move = Move_Clone(move);
    Move_WithPromotion(cloned_move, PieceType_Knight);
    MoveArray_Add(move_array, cloned_move);
}

void Pawn_Free(Piece* piece) {
    Pawn* pawn = (Pawn*)piece;
    free(pawn);
}
