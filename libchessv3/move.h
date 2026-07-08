#ifndef CH_MOVE_H
#define CH_MOVE_H

#include "common.h"
#include "vector2.h"

typedef struct {
  CH_PieceType pieceType;
  CH_Vector2 positionFrom;
  bool isTaking;
  CH_Vector2 positionTo;
  CH_PieceType promoteTo;
} CH_Move;

CH_Error CH_Move_ParseLAN(CH_Move *move, char *notation);

#endif