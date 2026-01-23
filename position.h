#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct Position {
    uint8_t i;
    uint8_t j;
} Position;

uint8_t Position_OutOfBound(Position* pos);
Position Position_Make(uint8_t i, uint8_t j);

#endif
