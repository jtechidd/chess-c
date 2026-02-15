#include "board.h"
#include "chess.h"
#include "move/move_array.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  int err;
  chess_t *chess;
  chess_new(&chess);
  board_debug(chess->board);
  move_array_t *move_array;
  if ((err = board_get_moves(&move_array, chess->board, chess->turn)) != CHESS_OK) {
    printf("%d\n", err);
    return 0;
  }
  move_array_debug(move_array);
  move_array_free(move_array);
  chess_free(chess);
  // move_t *move = move_array_get_index(move_array, 1);
  // board_apply_move(chess->board, move);
  // board_debug(chess->board);
  // chess_free(chess);
  return 0;
}