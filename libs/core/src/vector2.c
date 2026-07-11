#include "core/vector2.h"
#include "core/common.h"

CH_Vector2 CH_Vector2_Make(int8_t i, int8_t j) { return (CH_Vector2){i, j}; }

bool CH_Vector2_Equal(CH_Vector2 v1, CH_Vector2 v2) {
  return v1.i == v2.i && v1.j == v2.j;
}

CH_Vector2 CH_Vector2_Add(CH_Vector2 v1, CH_Vector2 v2) {
  return CH_Vector2_Make(v1.i + v2.i, v1.j + v2.j);
}

CH_Vector2 CH_Vector2_Sub(CH_Vector2 v1, CH_Vector2 v2) {
  return CH_Vector2_Make(v1.i - v2.i, v1.j - v2.j);
}

CH_Vector2 CH_Vector2_ScalMult(CH_Vector2 v, int8_t s) {
  return CH_Vector2_Make(v.i * s, v.j * s);
}

CH_Vector2 CH_Vector2_Inv(CH_Vector2 v) { return CH_Vector2_ScalMult(v, -1); }

CH_Vector2 CH_Vector2_FlipH(CH_Vector2 v) { return CH_Vector2_Make(v.i, -v.j); }

CH_Vector2 CH_Vector2_FlipV(CH_Vector2 v) { return CH_Vector2_Make(-v.i, v.j); }

bool CH_Vector2_IsPositionInBound(CH_Vector2 pos) {
  return pos.i >= 0 && pos.i < CH_BOARD_HEIGHT && pos.j >= 0 &&
         pos.j < CH_BOARD_WIDTH;
}
