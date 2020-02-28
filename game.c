/*
 * game.c
 *
 *  Created on: Feb 25, 2020
 *      Author: noada
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
#include "linkedList.h"
#include "fileParsing.h"
#include "solve.h"


/*print cell*/
void printCell(Cell *c){
	if(c->value!=0){
		int value= c->value;
		if(c->fixed==1){
			printf(" %2d.",value);
		}
		else{
			if(c->invalid==1 && mark_errors==1 ){
				printf(" %2d*",value);
			}
			else{
				printf(" %2d ",value);
			}
		}
	}
	else{
		printf("    ");
	}
}
/*print ----------- 4N+m+1 times*/
void printRowSepration(Game *game){
	int i=1;
	for(i=1;i<=(4*game->board_size)+game->num_of_rows_in_block+1;i++){
		printf("-");
	}
	printf("\n");
}
/*print part of the board game*/
void printBlockRow(int i, Game *game){
	int j=1,k=0;
	printRowSepration(game);
	for(k=i;k<i+game->num_of_rows_in_block;k++){
		for(j=1;j<=game->board_size;j++){
			if(j%game->num_of_columns_in_block==1){
				printf("|");
			}
			printCell(&(game->board[k][j]));
		}
		printf("|\n");
	}
}
/*print game board*/
void printBoard(Game *game){
	int k;
	for(k=0;k<game->num_of_columns_in_block;k++){
		printBlockRow(k*game->num_of_rows_in_block +1, game);
	}
	printRowSepration(game);
}

/*fill cell to be empty cell*/
void intilizeEmptyCell(Cell *c){
	c->fixed=0;
	c->value=0;
	c->invalid=0;

}
/*Create an empty board game*/
void intilizeEmptyBoard(Game *game){
	int i=1,j=1;
	game->board = (Cell**)malloc((game->board_size+1)*sizeof(Cell*));
	if(game->board==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	for(i=1;i<=game->board_size;i++){
		game->board[i]=(Cell*)malloc((game->board_size+1)*sizeof(Cell));
		if(game->board[i]==NULL){
			printf("ERROR: problem with memory allocation\n");
			exit(0);
		}
	}
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			intilizeEmptyCell(&game->board[i][j]);
		}
	}

}

Game* initializeGame(int num_of_rows_in_block, int num_of_columns_in_block){
	Game *game = (Game*)malloc(sizeof(Game));
	if(game==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	game->num_of_columns_in_block = num_of_rows_in_block;
	game->num_of_rows_in_block = num_of_columns_in_block;
	game->board_size = num_of_rows_in_block * num_of_columns_in_block;
	game->move_history = createList();
	intilizeEmptyBoard(game);
	return game;
}
/*check if the game board current row values are valid
 * return 1 if valid, else return 0
 */
int checkRowValid(int row ,Game *game){
	int i=1;
	int value;
	int *arr = (int*)calloc(game->board_size+1,sizeof(int));
	if(arr==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	for(i=1;i<=game->board_size;i++){
		if(game->board[row][i].value!=0){
			value=game->board[row][i].value;
			arr[value]++;
		}
	}
	for(i=1;i<=game->board_size;i++){
		if(arr[i]>1){
			free(arr);
			return 0;
		}
	}
	free(arr);
	return 1;
}
/*/*check if the game board current column values are valid
 * return 1 if valid, else return 0
 */
int checkColumnValid(int column,Game *game){
	int i=1;
	int value;
	int *arr =(int*) calloc(game->board_size+1,sizeof(int));
	if(arr==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	for(i=1;i<=game->board_size;i++){
		if(game->board[i][column].value!=0){
			value=game->board[i][column].value;
			arr[value]++;
		}
	}
	for(i=1;i<=game->board_size;i++){
		if(arr[i]>1){
			free(arr);
			return 0;
		}
	}
	free(arr);
	return 1;
}
/*/*check if the game board current block values are valid
 * return 1 if valid, else return 0
 */
int checkBlockValid(int row,int column,Game *game){
	int i,j,value;
	int *arr =(int*) calloc(game->board_size+1,sizeof(int));
	if(arr==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	for(i=row;i<row+game->num_of_rows_in_block;i++){
		for(j=column;j<column+game->num_of_columns_in_block;j++){
			if(game->board[i][j].value!=0){
				value=game->board[i][j].value;
				arr[value]++;
			}
		}
	}
	for(i=1;i<=game->board_size;i++){
		if(arr[i]>1){
			free(arr);
			return 0;
		}
	}
	free(arr);
	return 1;
}
/*check if the game board values are valid
 * return 1 if valid, else return 0
 */
int boardValueAreValid(Game *game){
	int i,correct,j;
	for(i=1;i<=game->board_size;i++){
		correct=checkColumnValid(i,game);
		if(correct==0){
			return 0;
		}
	}
	for(i=1;i<=game->board_size;i++){
		correct=checkRowValid(i,game);
		if(correct==0){
			return 0;
		}
	}
	for(i=1;i<=game->board_size;i=i+game->num_of_columns_in_block){
		for(j=1;j<=game->board_size;j=j+game->num_of_rows_in_block){
			correct=checkBlockValid(j, i, game);
			if(correct==0){
				return 0;
			}
		}
	}
	return 1;
}


/*sets cell in game board (z > 0)
 * if illegal input and returns returns 0
 * if cell is fixed prints: cell is fixed and returns 0
 * if cell exists in one of it's neighbors prints: value is invalid and returns 0
 * otherwise, keeps the cell and returns 1
 * */
void setCell(int x, int y, int z, Game *game, int start){
	Cell cur_cell;
	int max_value = game->num_of_columns_in_block*game->num_of_rows_in_block;
	if(game_status == Init){
		printf("Error: Set isn't available in Init mode\n");
		return;
	}
	if(y > game->board_size || y < 1){
		printf("Error: column number is incorrect\n");
	}
	if(x > game->board_size || x < 1){
		printf("Error: row number is incorrect\n");
	}

	if(z > max_value || z < 0){
		printf("Error: value is invalid, must be between %d and %d\n", 0, max_value);
	}
	if((game->board)[y][x].fixed && game_status == Solve){
		printf("Error: cell is fixed, can't change fixed cells in Solve mode\n");
		return;
	}
	cur_cell = (game->board)[y][x];
	insertToList(game->move_history, y, x, cur_cell.invalid, cur_cell.value, z, cur_cell.fixed, start);
	(game->board)[y][x].value = z;

}

void setCellsByMove(Game *game, Move *move, int undo){
	while(move->next != NULL && !move->next->move_start){
		move = move->next;
	}
	while(1){
		int y = move->row;
		int x = move->column;
		if(undo){
			(game->board)[y][x].value = move->before_value;
		}
		else{
			(game->board)[y][x].value = move->after_value;
		}
		(game->board)[y][x].fixed = move->fixed;
		(game->board)[y][x].invalid = move->invalid;
		if(move->move_start){
			break;
		}
		move = move->prev;
	}
}

void undoMove(Game *game){
	Move *move = undo(game->move_history);
	if(move == NULL){
		printf("Error: no moves to undo\n");
		return;
	}
	setCellsByMove(game, move, 1);

}

void redoMove(Game *game){
	Move *move = redo(game->move_history);
	if(move == NULL){
		printf("Error: no moves to redo\n");
		return;
	}
	setCellsByMove(game, move, 0);
}

void resetBoard(Game *game){
	Move *move = undo(game->move_history);
	while(move != NULL){
		setCellsByMove(game, move, 1);
		move = undo(game->move_history);
	}
}

void fixCellsWithValues(Game *game){
	int i, j;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if((game->board)[i][j].value != 0){
				(game->board)[i][j].fixed = 1;
			}
		}
	}

}

void freeGame(Game *game){
	int i;
	if(game == NULL){
		return;
	}
	for(i=1;i<=game->board_size;i++){
		free(game->board[i]);
	}
	free(game->board);
	clearList(game->move_history);
	free(game);

}

int invalidCell(Game *game, int row , int column, int ignore_not_fixed){
	int i,j,start_i,start_j,value;
	value=game->board[row][column].value;
	if(value==0){
		return 0;
	}
	else{
		for(i=1;i<=game->board_size;i++){
			if(i!=column){
				if((ignore_not_fixed==1 && game->board[row][i].fixed==1)|| ignore_not_fixed==0){
					if(game->board[row][i].value==value){
						printf("row");
						return 1;
					}
				}

			}
		}
		for(i=1;i<=game->board_size;i++){
			if(i!=row){
				if((ignore_not_fixed==1 &&game->board[i][column].fixed==1)|| ignore_not_fixed==0){
					if(game->board[i][column].value==value){
						printf("column");
						return 1;
					}
				}

			}
		}
		if( row%game->num_of_rows_in_block!=0){
			start_i=row- row%game->num_of_rows_in_block +1;
		}
		else{
			start_i=row-game->num_of_rows_in_block +1;
		}
		if(column%game->num_of_columns_in_block!=0){
			start_j=column- row%game->num_of_columns_in_block +1;
		}
		else{
			start_j=row-game->num_of_columns_in_block +1;
		}
		for(i=start_i; i<start_i+game->num_of_rows_in_block;i++){
			for(j=start_j; j<start_j+game->num_of_columns_in_block;j++){
				if(i!=row || j!=column){
					if((ignore_not_fixed==1 &&game->board[i][j].fixed==1)|| ignore_not_fixed==0){
						if(game->board[i][j].value==value){
							printf("block- i=%d, j=%d, row=%d, col=%d\n",i,j,row,column);
							return 1;
						}

					}
				}
			}
		}
	}
	return 0;
}
void markInvalidCells(Game *game){
	int i, j;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			game->board[i][j].invalid=invalidCell(game,i,j,0);

		}
	}
}
int oneValidOption(int *arr,Game *game){
	int count=0,i=1,value=0;
	for (i=1; i<= game->board_size;i++){
		if(arr[i]==0){
			count++;
			value=i;
		}
	}
	if(count==1){
		return value;
	}
	return 0;
}
void copyGame(Game *game, Game *copy_game){
	int i,j;
	copy_game->num_of_columns_in_block=game->num_of_columns_in_block;
	copy_game->num_of_rows_in_block=game->num_of_rows_in_block;
	copy_game->board_size=game->board_size;
	intilizeEmptyBoard(copy_game);
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			copy_game->board[i][j].value= game->board[i][j].value;
		}
	}
}
int autoFillBoard(Game *game){
	int i=1, j=1, value,start=1;
	Game *copy_game;
	int *arr_of_options;
	if(!boardValueAreValid(game)){
		printf("ERROR: board is invalid\n");
		return 0;
	}
	copy_game = (Game*)malloc(sizeof(Game));
	if(copy_game==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	copyGame(game,copy_game);
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if(copy_game->board[i][j].value==0){
				arr_of_options=(int*) calloc(game->board_size+1, sizeof(int));
				if(arr_of_options==NULL){
					printf("ERROR: problem with memory allocation\n");
					exit(0);
				}
				fillArrWithOption(arr_of_options,copy_game,i,j);
				if(stillHasOptionForCell(arr_of_options,copy_game)){
					value= oneValidOption(arr_of_options,copy_game);
					if(value!=0){
						setCell(j,i,value,game,start);
						start=0;
					}
				}
				free(arr_of_options);
			}

		}
	}
	freeGame(copy_game);
	return 1;
}

int checkFixedCells(Game *game){
	int i, j;
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			if((game->board)[i][j].fixed == 1){
				if(invalidCell(game, i , j, 1)){
					return 0;
				}
			}
		}
	}
	return 1;
}

Game* edit(char* fileName){
	Game *game;
	if(strcmp(fileName, "") == 0){
		game_status = Edit;
		return initializeGame(3, 3);
	}
	game = readFromFile(fileName, 0);
	if(game != NULL){
		markInvalidCells(game);
		game_status = Edit;
	}
	return game;
}

Game* solve(char* fileName){
	Game *game;
	game = readFromFile(fileName, 1);
	if(game != NULL){
		markInvalidCells(game);
	}
	return game;
}

void markErrors(int mark){
	if(mark == 0){
		mark_errors = 0;
	}
	else{
		mark_errors = 1;
	}
}


void exitGame(Game *game){
	freeGame(game);
	printf("exiting...");
	exit(0);
}


