#ifndef CHESS_NOTATION_H
#define CHESS_NOTATION_H

#include "defines.h"
#include "move.h"
#include <stdint.h>

typedef struct notation_t notation_t;

struct notation_t {
  uint8_t type;
  char data[NOTATION_DATA_MAX_SIZE];
};

void notation_init_from_move(notation_t *notation, piece_db_t *piece_db,
                             move_t *move);
void notation_init_from_position(notation_t *notation, vector2_t *position);

#endif