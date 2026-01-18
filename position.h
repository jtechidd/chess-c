#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct Position {
    int i;
    int j;
} Position;

uint8_t Position_OutOfBound(Position* pos);
Position Position_Make(int i, int j);

#endif
