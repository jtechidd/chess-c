#include "board.h"
#include "chess.h"

int main(int argc, char *argv[]) {
  chess_t *chess;
  chess_new(&chess);
  board_debug(chess->board);
  chess_free(chess);
  return 0;
}