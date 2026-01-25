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
#include "utils.h"

Board* board_new() {
    Board* board = (Board*)malloc(sizeof(Board));

    board_register_pieces_new_game(board);
    board_update_cells(board);

    return board;
}

Board* board_clone(Board* boardsrc) {
    Board* b = (Board*)malloc(sizeof(Board));

    board_register_pieces_from_board(b, boardsrc);
    board_update_cells(b);

    return b;
}

void board_clear_cells(Board* b) {
    memset(b->cells, 0, sizeof(b->cells));
}

void board_update_cells(Board* board) {
    board_clear_cells(board);
    for (PieceId id = 0; id < TOTAL_PIECES; id++) {
        Piece* piece = board_get_piece_by_id(board, id);
        if (piece->is_captured) {
            continue;
        }
        board->cells[piece->position.i][piece->position.j] = Cell_Make(1, piece->id);
    }
}

void board_register_pieces_new_game(Board* board) {
    board_register_piece(board, (Piece*)rook_new(PIECE_ID_BLACK_ROOK_QUEEN_SIDE, SIDE_BLACK, vector2_make(0, 0)));
    board_register_piece(board, (Piece*)knight_new(PIECE_ID_BLACK_KNIGHT_QUEEN_SIDE, SIDE_BLACK, vector2_make(0, 1)));
    board_register_piece(board, (Piece*)bishop_new(PIECE_ID_BLACK_BISHOP_QUEEN_SIDE, SIDE_BLACK, vector2_make(0, 2)));
    board_register_piece(board, (Piece*)queen_new(PIECE_ID_BLACK_QUEEN, SIDE_BLACK, vector2_make(0, 3)));
    board_register_piece(board, (Piece*)king_new(PIECE_ID_BLACK_KING, SIDE_BLACK, vector2_make(0, 4)));
    board_register_piece(board, (Piece*)bishop_new(PIECE_ID_BLACK_BISHOP_KING_SIDE, SIDE_BLACK, vector2_make(0, 5)));
    board_register_piece(board, (Piece*)knight_new(PIECE_ID_BLACK_KNIGHT_KING_SIDE, SIDE_BLACK, vector2_make(0, 6)));
    board_register_piece(board, (Piece*)rook_new(PIECE_ID_BLACK_ROOK_KING_SIDE, SIDE_BLACK, vector2_make(0, 7)));
    for (PieceId j = 0; j < 8; j++) {
        board_register_piece(board, (Piece*)pawn_new(PIECE_ID_BLACK_PAWN_1 + j, SIDE_BLACK, vector2_make(1, j)));
    }

    board_register_piece(board, (Piece*)rook_new(PIECE_ID_WHITE_ROOK_QUEEN_SIDE, SIDE_WHITE, vector2_make(7, 0)));
    board_register_piece(board, (Piece*)knight_new(PIECE_ID_WHITE_KNIGHT_QUEEN_SIDE, SIDE_WHITE, vector2_make(7, 1)));
    board_register_piece(board, (Piece*)bishop_new(PIECE_ID_WHITE_BISHOP_QUEEN_SIDE, SIDE_WHITE, vector2_make(7, 2)));
    board_register_piece(board, (Piece*)queen_new(PIECE_ID_WHITE_QUEEN, SIDE_WHITE, vector2_make(7, 3)));
    board_register_piece(board, (Piece*)king_new(PIECE_ID_WHITE_KING, SIDE_WHITE, vector2_make(7, 4)));
    board_register_piece(board, (Piece*)bishop_new(PIECE_ID_WHITE_BISHOP_KING_SIDE, SIDE_WHITE, vector2_make(7, 5)));
    board_register_piece(board, (Piece*)knight_new(PIECE_ID_WHITE_KNIGHT_KING_SIDE, SIDE_WHITE, vector2_make(7, 6)));
    board_register_piece(board, (Piece*)rook_new(PIECE_ID_WHITE_ROOK_KING_SIDE, SIDE_WHITE, vector2_make(7, 7)));
    for (PieceId j = 0; j < 8; j++) {
        board_register_piece(board, (Piece*)pawn_new(PIECE_ID_WHITE_PAWN_1 + j, SIDE_WHITE, vector2_make(6, j)));
    }
}

void board_register_pieces_from_board(Board* board_dst, Board* board_src) {
    for (PieceId piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
        Piece* piece = board_get_piece_by_id(board_src, piece_id);
        Piece* new_piece;
        switch (piece->type) {
            case PIECE_TYPE_ROOK:
                new_piece = (Piece*)rook_clone((Rook*)piece);
                break;
            case PIECE_TYPE_KNIGHT:
                new_piece = (Piece*)knight_clone((Knight*)piece);
                break;
            case PIECE_TYPE_BISHOP:
                new_piece = (Piece*)bishop_clone((Bishop*)piece);
                break;
            case PIECE_TYPE_QUEEN:
                new_piece = (Piece*)queen_clone((Queen*)piece);
                break;
            case PIECE_TYPE_KING:
                new_piece = (Piece*)king_clone((King*)piece);
                break;
            case PIECE_TYPE_PAWN:
                new_piece = (Piece*)pawn_clone((Pawn*)piece);
                break;
        }
        board_register_piece(board_dst, new_piece);
    }
}

void board_register_piece(Board* board, Piece* piece) {
    board->pieces[piece->id] = piece;
}

Piece* board_get_piece_by_id(Board* board, PieceId piece_id) { return board->pieces[piece_id]; }

Piece* board_get_piece_by_position(Board* board, Vector2 position) {
    if (!is_position_in_boundary(position)) {
        return NULL;
    }
    Cell cell = board->cells[position.i][position.j];
    if (!cell.has_piece) {
        return NULL;
    }
    PieceId piece_id = cell.piece_id;
    if (!is_piece_id_valid(piece_id)) {
        return NULL;
    }
    return board->pieces[piece_id];
}

uint8_t board_has_piece_on_position(Board* board, Vector2 position) {
    return board_get_piece_by_position(board, position) != NULL;
}

uint8_t board_can_take_position(Board* board, Piece* piece, Vector2 position) {
    Piece* piece_on_position = board_get_piece_by_position(board, position);
    return piece_on_position && piece_is_opposite(piece, piece_on_position);
}

MoveArray* board_get_moves(Board* board, Side side) {
    PieceId piece_id_start;
    PieceId piece_id_end;
    if (side == SIDE_WHITE) {
        piece_id_start = PIECE_ID_WHITE_ROOK_QUEEN_SIDE;
        piece_id_end = PIECE_ID_WHITE_PAWN_8;
    } else if (side == SIDE_BLACK) {
        piece_id_start = PIECE_ID_BLACK_ROOK_QUEEN_SIDE;
        piece_id_end = PIECE_ID_BLACK_PAWN_8;
    }

    MoveArray* all_moves = move_array_new();
    for (PieceId pieceid = piece_id_start; pieceid <= piece_id_end; pieceid++) {
        Piece* piece = board_get_piece_by_id(board, pieceid);
        if (piece->is_captured) {
            continue;
        }
        MoveArray* positional_moves = piece->piece_get_positional_moves(piece, board);
        for (size_t i = 0; i < positional_moves->length; i++) {
            Move* move = move_array_get_index(positional_moves, i);
            Board* cloned_board = board_clone(board);
            board_apply_move(cloned_board, move);
            if (board_is_king_get_attacked(cloned_board, side)) {
                move_free(move);
            } else {
                move_array_add(all_moves, move);
            }
            board_free(cloned_board);
        }

        move_array_shallow_free(positional_moves);
    }

    // TODO: add castling

    return all_moves;
}

void board_apply_move(Board* board, Move* move) {
    if (move->flags & MOVE_FLAGS_HAS_MOVING_PIECE) {
        PieceId piece_id = move->piece_id;
        Piece* piece = board_get_piece_by_id(board, piece_id);
        piece->position = move->position_to;

        Pawn* pawn;
        if (pawn = pawn_cast(piece)) {
            if (!pawn->has_been_moved) {
                pawn->has_been_moved = 1;
                pawn->first_time_moved = 1;
            }
            if (pawn->first_time_moved) {
                pawn->first_time_moved = 0;
            }
        }

        Rook* rook;
        if ((rook = pawn_cast(piece)) && !rook->has_been_moved) {
            rook->has_been_moved = 1;
        }

        King* king;
        if ((king = king_cast(piece)) && !king->has_been_moved) {
            king->has_been_moved = 1;
        }
    }
    if (move->flags & MOVE_FLAGS_HAS_TAKING_PIECE) {
        PieceId take_piece_id = move->take_piece_id;
        Piece* take_piece = board_get_piece_by_id(board, take_piece_id);
        take_piece->is_captured = 1;
    }
    if (move->flags & MOVE_FLAGS_HAS_PROMOTION) {
        PieceId piece_id = move->piece_id;
        Piece* piece = board_get_piece_by_id(board, piece_id);
        switch (move->promote_to) {
            case PIECE_TYPE_QUEEN:
                board_register_piece(board, (Piece*)queen_new(piece_id, piece->side, move->position_to));
                break;
            case PIECE_TYPE_ROOK:
                board_register_piece(board, (Piece*)rook_new(piece_id, piece->side, move->position_to));
                break;
            case PIECE_TYPE_BISHOP:
                board_register_piece(board, (Piece*)bishop_new(piece_id, piece->side, move->position_to));
                break;
            case PIECE_TYPE_KNIGHT:
                board_register_piece(board, (Piece*)knight_new(piece_id, piece->side, move->position_to));
                break;
        }
        piece->piece_free(piece);
    }
    if (move->flags & MOVE_FLAGS_HAS_CASTLING) {
        // TODO: add handle castling
    }
    board_update_cells(board);
}

uint8_t board_is_position_get_attacked(Board* board, Side side, Vector2 position) {
    if (board_is_position_get_attacked_by_pawn(board, side, position)) {
        return 1;
    }
    if (board_is_position_get_attacked_by_rook(board, side, position)) {
        return 1;
    }
    if (board_is_position_get_attacked_by_knight(board, side, position)) {
        return 1;
    }
    if (board_is_position_get_attacked_by_bishop(board, side, position)) {
        return 1;
    }
    if (board_is_position_get_attacked_by_queen(board, side, position)) {
        return 1;
    }
    if (board_is_position_get_attacked_by_king(board, side, position)) {
        return 1;
    }
    return 0;
}

uint8_t board_is_king_get_attacked(Board* board, Side side) {
    PieceId king_piece_id;
    if (side == SIDE_WHITE) {
        king_piece_id = PIECE_ID_WHITE_KING;
    } else if (side == SIDE_BLACK) {
        king_piece_id = PIECE_ID_BLACK_KING;
    }

    Piece* king_piece = board_get_piece_by_id(board, king_piece_id);

    King* king;
    if (!(king = king_cast(king_piece))) {
        return 0;
    }

    return board_is_position_get_attacked(board, king->piece.side, king->piece.position);
}

void board_free(Board* board) {
    for (PieceId piece_id = 0; piece_id < TOTAL_PIECES; piece_id++) {
        Piece* piece = board_get_piece_by_id(board, piece_id);
        piece->piece_free(piece);
    }
    free(board);
}

void board_debug(Board* board) {
    for (size_t i = 0; i < BOARD_HEIGHT; i++) {
        for (size_t j = 0; j < BOARD_WIDTH; j++) {
            Piece* piece = board_get_piece_by_position(board, vector2_make(i, j));
            if (!piece) {
                printf(".");
                continue;
            }
            switch (piece->type) {
                case PIECE_TYPE_ROOK:
                    printf("r");
                    break;
                case PIECE_TYPE_KNIGHT:
                    printf("k");
                    break;
                case PIECE_TYPE_BISHOP:
                    printf("b");
                    break;
                case PIECE_TYPE_QUEEN:
                    printf("q");
                    break;
                case PIECE_TYPE_KING:
                    printf("K");
                    break;
                case PIECE_TYPE_PAWN:
                    printf("p");
                    break;
            }
        }
        printf("\n");
    }
}