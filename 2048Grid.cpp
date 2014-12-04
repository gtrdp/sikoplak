#include "2048.h"
#include "2048Grid.h"
#include <iostream>

using namespace std;

void reflectGrid(int grid[][4], moves_t move){
	int temp;
	
	if(move == DOWN){
		for(int i=0;i<4;++i)
			for(int j=0;j<2;++j){
				temp = grid[i][j];
				grid[i][j] = grid[i][3-j];
				grid[i][3-j] = temp;
			}
	}
	if(move == LEFT){
		for(int i=0;i<3;++i)
			for(int j=(i+1);j<4;++j){
				temp = grid[i][j];
				grid[i][j] = grid[j][i];
				grid[j][i] = temp;
			}
	}
	
	if(move == RIGHT){
		for(int i=3;i>0;--i)
			for(int j={4-i};j<4;++j){
				temp = grid[i][j];
				grid[i][j] = grid[3-j][3-i];
				grid[3-j][3-i] = temp;
			}
	}
}

int moveGrid(int grid[][4], moves_t move){
	bool legal = false;
	int score = 0;
	int grid2[4][4];
	for(int i{0};i<4;++i){
		for(int j{0};j<4;++j){
			grid2[i][j] = grid[i][j];
		}
	}
	
	reflectGrid(grid,move);
	//cout << endl;
	for(int i{0};i<4;++i){
		int stack_pointer{0};
		int comp_pointer{0};
		while(comp_pointer<4){
			//for(int j{0};j<4;++j){cout << grid[i][j] << " ";}cout << endl;
			if(grid[i][comp_pointer] ==0){ ++comp_pointer;continue;}
			if(stack_pointer==comp_pointer){ ++comp_pointer;continue;}
			if(grid[i][stack_pointer]==0 && grid[i][comp_pointer] !=0){
				grid[i][stack_pointer] = grid[i][comp_pointer];
				grid[i][comp_pointer] = 0;
				legal = true;
				continue;
			}
			if(grid[i][stack_pointer]!=0 && grid[i][stack_pointer]==grid[i][comp_pointer] ){
				grid[i][stack_pointer] += grid[i][comp_pointer];
				grid[i][comp_pointer] = 0;
				legal = true;
				++stack_pointer;
				continue;
			}
			if(grid[i][stack_pointer]!=0 && grid[i][stack_pointer]!=grid[i][comp_pointer] ){
				++stack_pointer;
				continue;
			}
		}
		//cout << endl;
	}
	
	reflectGrid(grid,move);

	if(legal){
		return score;
	}else{
	/*
		cout << "Illegal move " <<move<< endl;
		for(int i{0};i<4;i++){
			for(int j{0};j<4;j++){
				cout << grid2[i][j] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for(int i{0};i<4;i++){
			for(int j{0};j<4;j++){
				cout << grid[i][j] << " ";
			}
			cout << endl;
		}
	*/
		return -1;
	}
}