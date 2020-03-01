
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "solve.h"
#include "structs.h"
#include "gurobi_c.h"
#include "gurobi.h"

int callGurobi(Game *game, int ILP){
	int value;
	if(!createEnv()){
		return -1;
	}
	if(!noMessages()){
		return -1;
	}
	if(!createObjAndSendToModel(game,ILP)){
		return -1;
	}
	if(!createBounds()){
		return -1;
	}
	if(!createEmptyModel()){
		return -1;
	}
	if(!sendToModel()){
		return -1;
	}

	if(!setCellConstraints(game)){
		return -1;
	}
	if(!setRowConstraints(game)){
		return -1;
	}
	if(!setColumnConstraints(game)){
		return -1;
	}
	if(!setBlockConstraints(game)){
		return -1;
	}
	if(!optimizeTheModel()){
		return -1;
	}
	if(!getSol()){
		return -1;
	}
	value=end();
	return value;
}
void validate(Game *game){
	int value=0,valid;
	valid=boardValueAreValid(game);
	if(valid==0){
		printf("there are errors on the board, no solution\n");
		return;
	}
	value=callGurobi(game,1);
	if(value==1){
		printf("solution found- board solvable\n");
	}
	if(value==0){
		printf("no solution found- board is not solvable\n");
	}
	if(value==-1){
		printf("problem accrued in gurobi\n");
	}
	freeGR();
}
void hint(Game *game, int row,int column){
	int value=0,valid;
	valid=boardValueAreValid(game);
	if(valid==0){
		printf("there are errors on the board, no solution, no hint\n");
		return;
	}
	if(game->board[row][column].fixed==1||game->board[row][column].value!=0){
		printf("cell not empty can't get an hint\n");
		return;
	}
	value=callGurobi(game,1);

	if(value==1){
		int value = getHint(row,column,game);
		printf("Hint- int cell<%d,%d> put %d.\n",column,row,value);
	}
	if(value==0){
		printf("no solution found- board is not solvable\n");
	}
	if(value==-1){
		printf("problem accrued in gurobi\n");
	}
	freeGR();
}

int guess(Game *game, float X){
	int value=0,valid;

	valid= autoFillBoard(game);
	if(valid==0){
		printf("there are errors on the board, no solution, no hint\n");
		return 0;
	}
	value = callGurobi(game,0);
	if(value==1){
		fillAllGuesses(game,X);
		freeGR();
		return 1;
	}
	if(value==0){
		printf("no solution found- board is not solvable\n");
		freeGR();
		return 0;
	}
	if(value==-1){
		printf("problem accrued in gurobi\n");
		freeGR();
		return 0;
	}
	return 0;
}
void guessHint(Game *game, int row, int column){
	int value=0,valid;
	valid=boardValueAreValid(game);
	if(valid==0){
		printf("there are errors on the board, no solution, no hint\n");
		return;
	}
	if(game->board[row][column].fixed==1||game->board[row][column].value!=0){
		printf("cell not empty can't get an hint\n");
		return;
	}
	value = callGurobi(game,0);
	if(value==1){
		printGuessHint(game, row,column);
		freeGR();
	}
	if(value==0){
		printf("no solution found- board is not solvable\n");
		freeGR();
	}
	if(value==-1){
		printf("problem accrued in gurobi\n");
		freeGR();

	}
}
int randomCellAndValue(Game *copy_game){
	int row, column;
	row=(rand()%copy_game->board_size)+1;
	column=(rand()%copy_game->board_size)+1;
	while(copy_game->board[row][column].value!=0){
		row=(rand()%copy_game->board_size)+1;
		column=(rand()%copy_game->board_size)+1;
	}

	return copy_game->board[row][column].value;
}
