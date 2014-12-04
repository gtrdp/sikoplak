#include "2048.h"

#ifndef N2048GRID_H
#define N2048GRID_H

enum moves_t{UP,DOWN,LEFT,RIGHT,NOP};
int moveGrid(int grid[][4], moves_t move);

#endif