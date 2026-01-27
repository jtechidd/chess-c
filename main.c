#include <stdio.h>

#include "board.h"
#include "chess.h"
#include "enums.h"

int main(int argc, char *argv[]) {
  chess_t *chess = Chess_New();
  board_debug(chess->board);
  printf("\n");

  move_array_t *moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
  printf("applying move %d\n", 9);
  board_apply_move(chess->board, move_array_get_index(moves, 9));
  move_array_free(moves);
  board_debug(chess->board);
  printf("\n");

  chess->turn = SIDE_BLACK;
  moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
  printf("applying move %d\n", 11);
  board_apply_move(chess->board, move_array_get_index(moves, 11));
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
  printf("applying move %d\n", 19);
  board_apply_move(chess->board, move_array_get_index(moves, 19));
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
  printf("applying move %d\n", 22);
  board_apply_move(chess->board, move_array_get_index(moves, 22));
  move_array_free(moves);
  board_debug(chess->board);
  printf("\n");

  chess->turn = SIDE_WHITE;
  moves = board_get_moves(chess->board, chess->turn);
  move_array_debug(moves);
}