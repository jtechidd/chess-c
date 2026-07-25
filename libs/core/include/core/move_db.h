#ifndef CH_MOVE_DB_H
#define CH_MOVE_DB_H

#include "core/common.h"
#include "core/move.h"

#define CH_MAX_TOTAL_MOVES 256

typedef struct {
  uint16_t total;
  ch_move_t moves[CH_MAX_TOTAL_MOVES];
} ch_move_db_t;

ch_move_t *ch_move_db_add_move(ch_move_db_t *move_db, ch_move_t move);
ch_move_t *ch_move_db_get_by_index(ch_move_db_t *move_db, uint16_t idx);

#endif