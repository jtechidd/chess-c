

#include "chess.h"
#include "board.h"
#include "utils.h"

chess_t *chess_new() {
  chess_t *chess = (chess_t *)xmalloc(sizeof(chess_t));
  chess->turn = SIDE_WHITE;
  chess->board = board_new();
  return chess;
}

void chess_free(chess_t *c) {
  board_free(c->board);
  xfree(c);
}
