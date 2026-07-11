#include "core/move.h"
#include "core/common.h"

CH_Error CH_Move_ParseLAN(CH_Move *move, char *notation) {
  size_t len;
  char *first, *last, *cur;
  CH_PieceType pieceType;
  uint8_t vertPosFrom, horizPosFrom, vertPosTo, horizPosTo;
  CH_Move parsedMove;

  memset(&parsedMove, 0, sizeof(CH_Move));

  len = strlen(notation);
  if (len < 2) {
    return CH_ERR_INVALID_NOTATION;
  }

  first = notation;
  last = notation + len - 1;
  cur = first;

  // 1. Parse piece type
  if ((pieceType = CH_CharToPieceType(*cur)) != CH_EMPTY) {
    cur++;
  }

  if (cur > last) {
    return CH_ERR_INVALID_NOTATION;
  }

  // 2. Parse position from
  if (cur <= last && CH_IsValidFile(*cur)) {
    horizPosFrom = CH_FileToHorizPos(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  if (cur <= last && CH_IsValidRank(*cur)) {
    vertPosFrom = CH_RankToVertPos(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  parsedMove.positionFrom = CH_Vector2_Make(vertPosFrom, horizPosFrom);

  // 3. Parse action
  if (cur > last) {
    return CH_ERR_INVALID_NOTATION;
  }

  if (*cur == 'x') {
    parsedMove.isTaking = true;
    cur++;
  } else {
    parsedMove.isTaking = false;
  }

  // 4. Parse position to
  if (cur <= last && CH_IsValidFile(*cur)) {
    horizPosTo = CH_FileToHorizPos(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  if (cur <= last && CH_IsValidRank(*cur)) {
    vertPosTo = CH_RankToVertPos(*cur);
    cur++;
  } else {
    return CH_ERR_INVALID_NOTATION;
  }

  parsedMove.positionTo = CH_Vector2_Make(vertPosTo, horizPosTo);

  // 5. Parse promotion
  if (cur <= last &&
      (parsedMove.promoteTo = CH_CharToPieceType(*cur)) != CH_EMPTY) {
    cur++;
  }

  memcpy(move, &parsedMove, sizeof(CH_Move));

  return CH_ERR_SUCCESS;
}
