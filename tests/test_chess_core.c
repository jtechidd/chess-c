#include <assert.h>

#include "core/chess.h"
#include "core/common.h"

void test_pawn_up2_failed_repeat() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e6") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_up_failed_occupied() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e5") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_up2_failed_occupied() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "h2h3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h3h4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e5e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "h4h5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_take_success() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4xd5") == CH_ERR_SUCCESS);
}

void test_pawn_take_failed() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e3xd4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e3xd4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e3xf4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "f2f4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e3xf4") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_enpassant_success() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e5xf6") == CH_ERR_SUCCESS);
}

void test_pawn_enpassant_failed() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d6d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e5xd6") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_enpassant_failed_do_later() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e4e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "a2a3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "a7a6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e5xf6") == CH_ERR_ILLEGAL_MOVE);
}

void test_pawn_promotion() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "f2f4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "g7g5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f4xg5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h7h6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "g5g6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h6h5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "g6g7") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h5h4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "g7xh8") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "g7xh8Q") == CH_ERR_SUCCESS);
}

void test_pawn_move_failed() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "e2e5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2e1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2d3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2d4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2d7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2f3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2f4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2f7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2f8") == CH_ERR_ILLEGAL_MOVE);
}

void test_rook() {
  ch_chess_t chess;

  ch_chess_init_standard(&chess);
  assert(ch_chess_apply_move_lan(&chess, "a2a4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ra1a5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ra1a4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ra1xa4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ra1a3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h7h5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ra3h3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ra3b4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ra3c5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh3xh5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh8xh5") == CH_ERR_SUCCESS);
}

void test_bishop() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bf1c4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bc4c5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc4b4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc4d4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc4c3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc4a2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc4xg8") == CH_ERR_SUCCESS);
}

void test_knight() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Kg1g3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kg1e2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kg1f3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Kg8g6") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kg8e7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kg8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Kb1b3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kb1c3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Kf6e8") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kf6d7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kf6h7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kf6d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Kc3d5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kc3xd5") == CH_ERR_SUCCESS);
}

void test_queen() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Qd1d3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1b3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1f3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qd1d4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1d5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1c3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1e3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1e1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1d3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qd3a4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd3g4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd3b4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd3f4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd3xf5") == CH_ERR_SUCCESS);
}

int main(int argc, char *argv[]) {
  test_pawn_up2_failed_repeat();
  test_pawn_up_failed_occupied();
  test_pawn_up2_failed_occupied();
  test_pawn_take_success();
  test_pawn_take_failed();
  test_pawn_enpassant_success();
  test_pawn_enpassant_failed();
  test_pawn_enpassant_failed_do_later();
  test_pawn_promotion();
  test_pawn_move_failed();

  test_rook();
  test_bishop();
  test_knight();

  return 0;
}