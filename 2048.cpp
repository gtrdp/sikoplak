#include <iostream>
#include <windows.h>
#include <memory>
#include "2048.h"
#include "2048IO.h"
#include "2048Grid.h"
#include "2048AI.h"
 
using namespace std;

void freeState(SOLVERSTATE* state){
	if(state->bitmapData){
		free(state->bitmapData);
	}
}

int main() {
	SOLVERSTATE state;
	SolverParam param;
	param.depth = 7;
	param.emptyTileBonus = 150;
	param.clusteringPenalty = 50;
	param.homogenityBonus = 40;
	param.ascColBonus = 5;
	param.ascRowBonus = 5;
	moves_t move;
	state.GRID_COLOR = 0xFFBBADA0;
	state.TILE_4_COLOR = 0x00C8E0ED;
	state.TILE_2_COLOR = 0x00DAE4EE;
	
	param.n=0;
	for(int y{0};y<4;++y){
		for(int x{0};x<4;++x){
			state.gridState[x][y] = 0;
		}
	}
	if(calibrateScreen(&state)){
		cout << "Calibration Failure";
		freeState(&state);
		return -1;
	}
	while(!gameOver(&state)){
		if(!getSmallTile(&state)) break;
		
		for(int y{0};y<4;++y){
			for(int x{0};x<4;++x){
				cout << state.gridState[x][y] << ' ';
			}
			cout << endl;
		}
		move = decideMove(state.gridState, &param);
		//break;
		actuateMoves(move);
	}
	freeState(&state);
	return 0;
} 