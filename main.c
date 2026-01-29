#include <stdio.h>

#include "board.h"
#include "chess.h"
#include "enums.h"
#include "vector2.h"

int main(int argc, char *argv[]) {
  chess_t *chess = Chess_New();
  board_debug(chess->board);
  printf("\n");

  move_array_t *moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
  printf("applying move %d\n", 13);
  board_apply_move(chess->board, move_array_get_index(moves, 13));
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
  printf("applying move %d\n", 12);
  board_apply_move(chess->board, move_array_get_index(moves, 12));
  move_array_free(moves);
  board_debug(chess->board);
  printf("\n");

  chess->turn = SIDE_BLACK;
  moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
  printf("applying move %d\n", 1);
  board_apply_move(chess->board, move_array_get_index(moves, 1));
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
  printf("applying move %d\n", 16);
  board_apply_move(chess->board, move_array_get_index(moves, 16));
  move_array_free(moves);
  board_debug(chess->board);
  printf("\n");

  printf("%d\n", board_is_position_get_attacked(chess->board, SIDE_WHITE, vector2_make(7, 4)));

  chess->turn = SIDE_WHITE;
  moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
  printf("applying move %d\n", 5);
  board_apply_move(chess->board, move_array_get_index(moves, 5));
  move_array_free(moves);
  board_debug(chess->board);
  printf("\n");
}