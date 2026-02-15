#include <locale.h>

#include "board.h"
#include "chess.h"
#include "move/move_array.h"

int main(int argc, char *argv[]) {
  setlocale(LC_CTYPE, "");

  chess_t *chess = chess_new();
  board_debug(chess->board);
  move_array_t *move_array = board_get_moves(chess->board, chess->turn);
  move_array_debug(move_array);
  move_array_free(move_array);
  chess_free(chess);
}