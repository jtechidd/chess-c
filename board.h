#ifndef BOARD_H
#define BOARD_H

#include <stdint.h>

#include "cell.h"
#include "move/move.h"
#include "pieces/piece.h"

#define BOARD_WIDTH 8
#define BOARD_HEIGHT 8
#define TOTAL_PIECES 32

typedef struct Board {
    Cell cells[BOARD_HEIGHT][BOARD_WIDTH];
    Piece* pieces[TOTAL_PIECES];
} Board;

Board* board_new();
Board* board_clone(Board*);
void board_clear_cells(Board*);
void board_update_cells(Board*);
void board_register_pieces_new_game(Board*);
void board_register_pieces_from_board(Board*, Board*);
void board_register_piece(Board*, Piece*);
Piece* board_get_piece_by_id(Board*, PieceId);
Piece* board_get_piece_by_position(Board*, Vector2);
uint8_t board_has_piece_on_position(Board*, Vector2);
uint8_t board_can_take_position(Board*, Piece*, Vector2);
MoveArray* board_get_moves(Board*, Side);
void board_apply_move(Board*, Move*);
uint8_t board_is_king_in_danger(Board*, Side);
void board_free(Board*);
void board_debug(Board*);
uint8_t is_piece_id_valid(PieceId);
uint8_t is_position_in_boundary(Vector2);
uint8_t is_position_top(Vector2);
uint8_t is_position_bottom(Vector2);

#endif