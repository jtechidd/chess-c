#include <stdio.h>

#include "board.h"
#include "chess.h"

int main(int argc, char* argv[]) {
    chess_t* chess = Chess_New();
    board_debug(chess->board);
    printf("\n");

    move_array_t* moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 9);
    board_apply_move(chess->board, move_array_get_index(moves, 9));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");
    
    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 13);
    board_apply_move(chess->board, move_array_get_index(moves, 13));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");
    
    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 18);
    board_apply_move(chess->board, move_array_get_index(moves, 18));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");
    
    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 22);
    board_apply_move(chess->board, move_array_get_index(moves, 22));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 14);
    board_apply_move(chess->board, move_array_get_index(moves, 14));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 14);
    board_apply_move(chess->board, move_array_get_index(moves, 14));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 7);
    board_apply_move(chess->board, move_array_get_index(moves, 7));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 41);
    board_apply_move(chess->board, move_array_get_index(moves, 41));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");
    
    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 1);
    board_apply_move(chess->board, move_array_get_index(moves, 1));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 25);
    board_apply_move(chess->board, move_array_get_index(moves, 25));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 23);
    board_apply_move(chess->board, move_array_get_index(moves, 23));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 36);
    board_apply_move(chess->board, move_array_get_index(moves, 36));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_WHITE;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    printf("applying move %d\n", 16);
    board_apply_move(chess->board, move_array_get_index(moves, 16));
    move_array_free(moves);
    board_debug(chess->board);
    printf("\n");

    chess->turn = SIDE_BLACK;
    moves = board_get_moves(chess->board, chess->turn);
    move_array_debug(moves);
    // printf("applying move %d\n", 16);
    // board_apply_move(chess->board, move_array_get_index(moves, 16));
    // move_array_free(moves);
    // board_debug(chess->board);
    printf("\n");
}