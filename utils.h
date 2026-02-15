#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stdlib.h>

#include "enums.h"
#include "vector2.h"

void *xmalloc(size_t);
void *xrealloc(void *, size_t);
void xfree(void *);
bool is_position_in_bound(vector2_t);
bool is_position_top(vector2_t);
bool is_position_bottom(vector2_t);
bool is_opposite_side(side_t, side_t);

#endif