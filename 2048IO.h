#include "2048.h"
#include "2048Grid.h"
#ifndef N2048IO_H
#define N2048IO_H

void captureScreen(SOLVERSTATE* state);
int calibrateScreen(SOLVERSTATE* state);
bool getSmallTile(SOLVERSTATE* state);
bool gameOver(SOLVERSTATE* state);
void actuateMoves(moves_t move);

#endif