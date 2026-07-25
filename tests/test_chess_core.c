#include <assert.h>

#include "core/chess.h"
#include "core/common.h"
#include "core/move.h"
#include "core/move_db.h"

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

  assert(ch_chess_apply_move_lan(&chess, "Ng1g3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng1e2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng1f3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ng8g6") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng8e7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb1b3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nb1c3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nf6e8") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nf6d7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nf6h7") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nf6d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nc3d5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nc3xd5") == CH_ERR_SUCCESS);
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

void test_king() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Ke1e2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke1d2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke1d1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke1f2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke1f1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke1e2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke8e7") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke2e4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2f4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2g4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2d4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2c4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2d2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2d1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2f2") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2f1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ke2d3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke7f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Kd3f3") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kd3e4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Kd3e3") == CH_ERR_SUCCESS);
}

void test_king_castling_king_side() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng1f3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb8c6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bf1b5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ng8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_SUCCESS);
}

void test_king_castling_queen_side() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc1f4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bc8f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nb1c3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb8c6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1d2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qd8d7") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_SUCCESS);
}

void test_king_castling_king_side_invalid_rook_moved() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Ng1f3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb8c6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bf1b5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ng8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h2h3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "h7h6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh1h2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh8h7") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh2h1") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rh7h8") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
}

void test_king_castling_king_side_invalid_unsafe() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "f2f4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "f4xe5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qd8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ng1h3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "c7c5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bf1e2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "b7b5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qf6e6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1g1") == CH_ERR_SUCCESS);
}

void test_king_castling_queen_side_invalid_rook_moved() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc1f4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bc8f5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nb1c3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb8c6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Qd1d2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Qd8d7") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ra1b1") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "a7a6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Rb1a1") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "a6a5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ke1c1") == CH_ERR_ILLEGAL_MOVE);
}

void test_king_pin() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  assert(ch_chess_apply_move_lan(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nb1c3") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Bf8b4") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nc3d5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nc3b5") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nc3a4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nc3e4") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Nc3b1") == CH_ERR_ILLEGAL_MOVE);
  assert(ch_chess_apply_move_lan(&chess, "Bc1d2") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Ng8f6") == CH_ERR_SUCCESS);
  assert(ch_chess_apply_move_lan(&chess, "Nc3d5") == CH_ERR_SUCCESS);
}

void test_fill_moves_ruy_lopez() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  ch_chess_apply_move_lan(&chess, "e2e4");
  ch_chess_apply_move_lan(&chess, "e7e5");
  ch_chess_apply_move_lan(&chess, "Ng1f3");
  ch_chess_apply_move_lan(&chess, "Nb8c6");
  ch_chess_apply_move_lan(&chess, "Bf1b5");
  ch_chess_apply_move_lan(&chess, "Ng8f6");

  ch_chess_print_board(&chess);

  ch_move_db_t move_db;
  ch_chess_fill_moves(&chess, &move_db);

  ch_move_t *move;
  char move_str[16];
  printf("%d\n", move_db.total);
  for (uint16_t i = 0; i < move_db.total; i++) {
    move = ch_move_db_get_by_index(&move_db, i);
    ch_move_to_str(move, move_str, 16);
    printf("%s\n", move_str);
  }
}

void test_fill_moves_fools_mate() {
  ch_chess_t chess;
  ch_chess_init_standard(&chess);

  ch_chess_apply_move_lan(&chess, "g2g4");
  ch_chess_apply_move_lan(&chess, "e7e5");
  ch_chess_apply_move_lan(&chess, "f2f3");
  ch_chess_apply_move_lan(&chess, "Qd8h4");

  ch_chess_print_board(&chess);

  ch_move_db_t move_db;
  ch_chess_fill_moves(&chess, &move_db);

  ch_move_t *move;
  char move_str[16];
  printf("%d\n", move_db.total);
  for (uint16_t i = 0; i < move_db.total; i++) {
    move = ch_move_db_get_by_index(&move_db, i);
    ch_move_to_str(move, move_str, 16);
    printf("%s\n", move_str);
  }
}

int main(int argc, char *argv[]) {

  printf("%ld\n", sizeof(ch_chess_t));
  printf("%ld\n", sizeof(ch_move_db_t));
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
  test_queen();
  test_king();
  test_king_castling_king_side();
  test_king_castling_king_side_invalid_rook_moved();
  test_king_castling_king_side_invalid_unsafe();
  test_king_castling_queen_side();
  test_king_castling_queen_side_invalid_rook_moved();
  test_king_pin();

  test_fill_moves_ruy_lopez();
  // test_fill_moves_fools_mate();

  return 0;
}