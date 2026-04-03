#include "chess.h"

int main(int argc, char *argv[]) {
  chess_t chess;

  chess_init_standard(&chess);
  chess_debug(&chess);
  chess_fill_moves(&chess);
  return 0;
}