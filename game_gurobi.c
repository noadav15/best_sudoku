
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
		printf("there is erroneous\n");
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
		printf("ERROR: a problem has accrued with Gurobi\n");
	}
	freeGR();
}
void hint(Game *game, int row,int column){
	int value=0,valid;
	valid=boardValueAreValid(game);
	if(valid==0){
		printf("the board is erroneous\n");
		return;
	}
	if(game->board[row][column].fixed==1||game->board[row][column].value!=0){
		printf("this cell already contains a value\n");
		return;
	}
	value=callGurobi(game,1);

	if(value==1){
		int value = getHint(row,column,game);
		printf("Hint- set cell<%d,%d> to %d.\n",column,row,value);
	}
	if(value==0){
		printf("no solution found- board is unsolvable\n");
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
		printf("the board is erroneous\n");
		return 0;
	}
	value = callGurobi(game,0);
	if(value==1){
		fillAllGuesses(game,X);
		freeGR();
		return 1;
	}
	if(value==0){
		printf("no solution found- board is unsolvable\n");
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
		printf("the board is erroneous\n");
		return;
	}
	if(game->board[row][column].fixed==1||game->board[row][column].value!=0){
		printf("this cell already contains a value\n");
		return;
	}
	value = callGurobi(game,0);
	if(value==1){
		printGuessHint(game, row,column);
		freeGR();
	}
	if(value==0){
		printf("no solution found- board is unsolvable\n");
		freeGR();
	}
	if(value==-1){
		printf("problem accrued in gurobi\n");
		freeGR();

	}
}
int *randomCell(Game *copy_game){
	int row, column;
	int *result=(int*)malloc(2*sizeof(int));
	row=(rand()%copy_game->board_size)+1;
	column=(rand()%copy_game->board_size)+1;
	while(copy_game->board[row][column].value!=0){
		row=(rand()%copy_game->board_size)+1;
		column=(rand()%copy_game->board_size)+1;
	}
	result[0]=row;
	result[1]=column;
	return result;
}
void emptyYCells(Game *copy_game, int Y){
	int row, column,i;
	for(i=1;i<=Y;i++){
		row=(rand()%copy_game->board_size)+1;
		column=(rand()%copy_game->board_size)+1;
		while(copy_game->board[row][column].value==0){
			row=(rand()%copy_game->board_size)+1;
			column=(rand()%copy_game->board_size)+1;
		}
		copy_game->board[row][column].value=0;
		copy_game->board[row][column].fixed=0;
		copy_game->board[row][column].invalid=0;
	}
}
int fillXCells(Game *copy_game, int X){
	int value=0,count,*indexes,random_index,count_X;
	int *arr_of_option;
	for(count_X=1;count_X<=X ;count_X++){
		arr_of_option= (int*)calloc(copy_game->board_size+1,sizeof(int));
		indexes = randomCell(copy_game);
		fillArrWithOption(arr_of_option,copy_game,indexes[0],indexes[1]);
		if(stillHasOptionForCell(arr_of_option,copy_game)){
			count=0;
			for(value=1;value<=copy_game->board_size;value++){
				if(arr_of_option[value]==0){
					count++;
				}
			}
			random_index=rand()%count;
			count =0;
			for(value=1;value<=copy_game->board_size;value++){
				if(arr_of_option[value]==0){
					if(count==random_index){
						copy_game->board[indexes[0]][indexes[1]].value=value;
					}
					count++;
				}
			}
		}
		else{
			free(indexes);
			free(arr_of_option);
			return 0;
		}
		free(indexes);
		free(arr_of_option);
	}
	return 1;
}

int generate(Game *game, int X,int Y){
	int value=0,valid,count,i,j,k, first=1;
	Game *copy_game ;
	Game *game_sol ;
	valid=boardValueAreValid(game);
	if(valid==0){
		printf("there are errors on the board, can't generate\n");
		return 0;
	}
	count= numberOfEmptyCell(game);
	if(count<X){
		printf("there are less than %d empty cells, can't generate\n",X);
		return 0;
	}
	for(i=0;i<1000;i++){
		copy_game =(Game*)malloc(sizeof(Game));
		copyGame(game,copy_game);
		/*fill X cells*/
		valid= fillXCells(copy_game,X);
		if(valid){
			/*after fill X cell valid do ILP*/
			value=callGurobi(copy_game,1);
			if(value==1){
				game_sol= findHintBoard(copy_game);
				/*empty Y cells*/
				emptyYCells(game_sol,Y);
				for(j=1;j<=game->board_size;j++){
					for(k=1;k<=game->board_size;k++){
						setCell(j,k,game_sol->board[j][k].value,game,first);
						first=0;
					}
				}
				freeGame(copy_game);
				freeGame(game_sol);
				freeGR();
				return 1;
			}
			freeGR();
		}
		freeGame(copy_game);
	}
	printf("generate didn't find any cells to fill\n");
	return 0;
}




