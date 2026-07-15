#include <assert.h>

#include "core/chess.h"
#include "core/common.h"

void testPawnInvalidDoubleStep() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4e6") == CH_ERR_ILLEGAL_MOVE);
}

void testPawnStepUpOccupied() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4e5") == CH_ERR_ILLEGAL_MOVE);
}

void testPawnDoubleStepUpOccupied() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h2h3") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h3h4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e5e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h4h5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4e3") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_ILLEGAL_MOVE);
}

void testPawnTake() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4xd5") == CH_ERR_SUCCESS);
}

void testPawnInvalidTake() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e3") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e3xd4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d2d4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e3xd4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e3xf4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f2f4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e3xf4") == CH_ERR_ILLEGAL_MOVE);
}

void testPawnEnpassantSuccess() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d7d5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e5xf6") == CH_ERR_SUCCESS);
}

void testPawnInvalidEnpassant() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d7d6") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e4e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "d6d5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e5xd6") == CH_ERR_ILLEGAL_MOVE);
}

void testPawnPromotion() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f2f4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "g7g5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f4xg5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h7h6") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "g5g6") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h6h5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "g6g7") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h5h4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "g7xh8") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "g7xh8Q") == CH_ERR_SUCCESS);
}

void testPawnInvalidMove() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e5") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2e1") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2d3") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2d4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2d7") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2f3") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2f4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2f7") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e2f8") == CH_ERR_ILLEGAL_MOVE);
}

void testRookMove() {
  CH_Chess chess;

  CH_Chess_InitStandard(&chess);
  assert(CH_Chess_ApplyMoveLAN(&chess, "a2a4") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "e7e5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra1a5") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra1a4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra1xa4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra1a3") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "h7h5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra3h3") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra3b4") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Ra3c5") == CH_ERR_ILLEGAL_MOVE);
  assert(CH_Chess_ApplyMoveLAN(&chess, "f7f5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Rh3xh5") == CH_ERR_SUCCESS);
  assert(CH_Chess_ApplyMoveLAN(&chess, "Rh8xh5") == CH_ERR_SUCCESS);
}

int main(int argc, char *argv[]) {
  testPawnInvalidDoubleStep();
  testPawnStepUpOccupied();
  testPawnDoubleStepUpOccupied();
  testPawnTake();
  testPawnInvalidTake();
  testPawnEnpassantSuccess();
  testPawnInvalidEnpassant();
  testPawnPromotion();
  testPawnInvalidMove();

  testRookMove();

  return 0;
}