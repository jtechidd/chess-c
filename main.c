#include "board.h"
#include "chess.h"

int main(int argc, char *argv[]) {
  chess_t *chess = chess_new();
  board_debug(chess->board);
}