#ifndef POSITION_H
#define POSITION_H

#include <stdint.h>

typedef struct Position {
    uint8_t i;
    uint8_t j;
} Position;

Position Position_Make(uint8_t i, uint8_t j);

#endif
