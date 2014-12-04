#include "2048.h"
#include "2048Grid.h"
#include "2048AI.h"
#include <limits>
#include <algorithm>
#include <iostream>
#include <cmath>

using namespace std;
float heuristics(int grid[][4], const SolverParam *param){
	float score{0};
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			if(grid[i][j]==0){score+=param->emptyTileBonus;} //Empty tile score
			if(grid[i][j]>4){score-=param->tilePenalty*(10-log2(grid[i][j]));}//occupancy penalty
			if(grid[i][j]<5){score-=param->smallTilePenalty;}
		}
	}
	for(int i{0};i<4;++i){
		for(int j{1};j<4;++j){
			//row wise
			if(grid[j][i] !=0){
				if(grid[j-1][i]!=0){score-= param->clusteringPenalty*abs(log2(grid[j-1][i])-log2(grid[j][i])+1);} // clustering
				if(grid[j-1][i]==grid[j][i]){score+= param->homogenityBonus;} //homogenity
			}
			//columnwise
			if(grid[i][j] !=0){
				if(grid[i][j-1]==grid[i][j]){score+= param->homogenityBonus;} // homogenity
				if(grid[i][j-1]!=0){score-= param->clusteringPenalty*abs(log2(grid[i][j-1])-log2(grid[i][j])+1);} //clustering
			}
		}
	}
	//ascending-descending
	int dec_rhold;
	int asc_rhold;
	bool rdec;
	bool rasc;
	int dec_chold;
	int asc_chold;
	bool cdec;
	bool casc;
	int colOr{0};
	int rowOr{0};
	
	for(int i{0};i<4;++i){
		asc_rhold = -1;
		dec_rhold = 100000;
		rdec = true;
		rasc = true;
		asc_chold = -1;
		dec_chold = 100000;
		cdec = true;
		casc = true;
		for(int j{1};j<4;++j){
			if(grid[i][j]!=0){
				if(grid[i][j] >= asc_rhold)
					asc_rhold = grid[i][j];
				else
					rasc =false;
				if(grid[i][j] <= dec_rhold)
					dec_rhold = grid[i][j];
				else
					rdec =false;
			}
			if(grid[j][i]!=0){
				if(grid[j][i] >= asc_chold)
					asc_chold = grid[j][i];
				else
					casc =false;
				if(grid[j][i] <= dec_chold)
					dec_rhold = grid[j][i];
				else
					cdec =false;
			}
		}
		if(!(cdec ||casc))score -= param->unorderedColPenalty;
		if(!(rdec ||rasc))score -= param->unorderedRowPenalty;
		if(cdec) {score+=asc_chold <1?param->emptyColBonus:param->descColBonus*log2(asc_chold);--colOr;}
		if(rdec) {score+=asc_rhold <1?param->emptyRowBonus:param->descRowBonus*log2(asc_rhold);--rowOr;}
		if(casc) {score+=asc_chold <1?param->emptyColBonus:param->ascColBonus*log2(asc_chold);++colOr;}
		if(rasc) {score+=asc_rhold <1?param->emptyRowBonus:param->ascRowBonus*log2(asc_rhold);++rowOr;}
	}
	score += param->rowsOrientationBonus*abs(rowOr);
	score += param->colsOrientationBonus*abs(colOr);
	/*if(score < 0){
		for(int i{0};i<4;i++){
			for(int j{0};j<4;j++){
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
	}*/
	return score;
}

bool aiTerminal(int grid[][4]){
	//Check for an empty file
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			if(grid[i][j]==0){return false;}
		}
	}
	// Check if there is an adjacent tile with same value 
	for(int i{0};i<4;++i){
		for(int j{1};j<4;++j){
			if(grid[i][j]==grid[i][j-1]){return false;}
			if(grid[j-1][i]==grid[j-1][i]){return false;}
		}
	}
	return true;
}

bool advTerminal(int grid[][4]){
	// check if there is any empty spot in the grid
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			if(grid[i][j]==0){return false;}
		}
	}
	return true;
}

bool aiGenNode(int grid[][4],int gridTemp[][4],int index){
	moves_t move;
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			gridTemp[i][j] = grid[i][j];
		}
	}
	if(index == 0) move=UP;
	if(index == 1) move=DOWN;
	if(index == 2) move=LEFT;
	if(index == 3) move=RIGHT;
	return (moveGrid(gridTemp,move)!=-1);
}

bool advGenNode(int grid[][4],int gridTemp[][4],int index){
	int tile;
	int y;
	int x;
	x = index %4;
	y = index/4;
	tile = index > 7 ? 4:2;
	moves_t move;
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			gridTemp[i][j] = grid[i][j];
		}
	}
	if(gridTemp[y][x]==0){
		gridTemp[y][x] = tile;
		return true;
	}else
		return false;
}

float minimaxSearch(int grid[][4],int depth,float alpha, float beta,bool aiTurn, SolverParam *param){
	float score;
	if(depth==0) return heuristics(grid,param);
	int gridTemp[4][4];
	if(aiTurn){
		if(!aiTerminal(grid)){
			for(int i{0};i<4;++i){
				if(aiGenNode(grid,gridTemp,i)){
					score = minimaxSearch(gridTemp,depth-1,alpha,beta,false,param);
					if(param->depth == depth) {param->scoreMatrix[i] = score;} 
					alpha = max(alpha,score);
					if(beta <= alpha) break;
				}
			}
			return alpha;
		}else
			return heuristics(grid,param);
	}else{
		if(!advTerminal(grid)){
			for(int i{0};i<16;++i){
				if(advGenNode(grid,gridTemp,i)){
					beta = min(beta,minimaxSearch(gridTemp,depth-1,alpha, beta,true,param));
					if(beta<=alpha) break;
				}
			}
			return beta;
		}else{
			beta = min(beta,minimaxSearch(gridTemp,depth-1,alpha,beta,true,param));
			return beta;
		}
	}
}

moves_t decideMove(int grid[][4],SolverParam* param){
	float score;
	int index;
	moves_t move;
	param->n = (param->n)+1;
	for(int i{0};i<4;++i) param->scoreMatrix[i] = -numeric_limits<float>::infinity();
	
	minimaxSearch(grid,param->depth, -numeric_limits<float>::infinity(),numeric_limits<float>::infinity(),true,param);
	score = param->scoreMatrix[0];
	index = 0;
	for(int i{0};i<4;++i) {cout << param->scoreMatrix[i] <<" ";}
	cout << endl;
	for(int i{1};i<4;++i) if(param->scoreMatrix[i]>score) {score=param->scoreMatrix[i];index =i;} 
	
	if(index == 0) move=UP;
	if(index == 1) move=DOWN;
	if(index == 2) move=LEFT;
	if(index == 3) move=RIGHT;
	if(score== -numeric_limits<float>::infinity()) move=NOP;
	
	//int grid[4][4] {{2,0,2,2},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
	//moveGrid(grid,UP);
	moveGrid(grid,move);
	cout << endl;
	for(int y{0};y<4;++y){
		for(int x{0};x<4;++x){
			cout << grid[x][y] << ' ';
		}
		cout << endl;
	}
	
	return move;
}