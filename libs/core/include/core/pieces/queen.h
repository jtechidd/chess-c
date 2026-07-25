#ifndef CH_QUEEN_H
#define CH_QUEEN_H

#include "core/common.h"
#include "core/vector2.h"

void ch_chess_spawn_queen(ch_chess_t *chess, ch_side_t side,
                                      ch_vector2_t position);
bool ch_chess_is_position_safe_from_queen(ch_chess_t *chess,
                                           ch_vector2_t position);
#endif