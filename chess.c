#include "chess.h"

#include <stdlib.h>

#include "board.h"
#include "errno.h"

int chess_new(chess_t **chess_out) {
  if (!chess_out) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  int err;
  chess_t *chess = (chess_t *)malloc(sizeof(chess_t));
  chess->turn = SIDE_WHITE;
  if ((err = board_new(&chess->board)) != CHESS_OK) {
    return err;
  }
  *chess_out = chess;
  return CHESS_OK;
}

int chess_free(chess_t *chess) {
  if (!chess) {
    return CHESS_ERROR_INVALID_ARGS;
  }
  if (chess->board != NULL) {
    board_free(chess->board);
    chess->board = NULL;
  }
  free(chess);
  return CHESS_OK;
}
