#ifndef N2048AI_H
#define N2048AI_H
#include "2048.h"
#include "2048Grid.h"
typedef struct SolverParam_t{
	int depth;
	float scoreMatrix[4];
	float homogenityBonus{0};
	float rowsOrientationBonus{0};
	float colsOrientationBonus{0};
	float emptyTileBonus{0};
	float tilePenalty{0};
	float smallTilePenalty{0};
	float clusteringPenalty{0};
	float unorderedColPenalty{0};
	float unorderedRowPenalty{0};
	float emptyColBonus{0};
	float emptyRowBonus{0};
	float descColBonus{0};
	float descRowBonus{0};
	float ascColBonus{0};
	float ascRowBonus{0};
	int n;
} SolverParam;

moves_t decideMove(int grid[][4],SolverParam* param);

#endif