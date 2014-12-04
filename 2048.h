#ifndef N2048_H
#define N2048_H
#include <windows.h>

typedef struct solverStateStruct{
	DWORD GRID_COLOR;
	DWORD TILE_2_COLOR;
	DWORD TILE_4_COLOR;
	int screenWidth;
	int screenHeight;
	int boxXStart;
	int boxXEnd;
	int boxYStart;
	int boxYEnd;
	int gridState[4][4];
	DWORD *bitmapData;
} SOLVERSTATE;

void freeState(SOLVERSTATE* state);

#endif