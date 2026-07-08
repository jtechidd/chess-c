#ifndef CH_VECTOR2_H
#define CH_VECTOR2_H

#include "common.h"

typedef struct {
  int8_t i;
  int8_t j;
} CH_Vector2;

CH_Vector2 CH_Vector2_Make(int8_t i, int8_t j);
bool CH_Vector2_Equal(CH_Vector2 v1, CH_Vector2 v2);
CH_Vector2 CH_Vector2_Add(CH_Vector2 v1, CH_Vector2 v2);
CH_Vector2 CH_Vector2_Sub(CH_Vector2 v1, CH_Vector2 v2);
CH_Vector2 CH_Vector2_ScalMult(CH_Vector2 v, int8_t s);
CH_Vector2 CH_Vector2_Inv(CH_Vector2 v);
CH_Vector2 CH_Vector2_FlipH(CH_Vector2 v);
CH_Vector2 CH_Vector2_FlipV(CH_Vector2 v);
bool CH_Vector2_IsPositionInBound(CH_Vector2 pos);

#endif