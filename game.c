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

/*print cell*/
void printCell(Cell *c){
	if(c->value!=0){
		int value= c->value;
		if(c->fixed==1){
			printf(" %2d.",value);
		}
		else{
			if(c->invalid==1){
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
	for(i=1;i<=game->board_size;i++){
		game->board[i]=(Cell*)malloc((game->board_size+1)*sizeof(Cell));
	}
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
			intilizeEmptyCell(&game->board[i][j]);
		}
	}

}

/*check if the game board current row values are valid
 * return 1 if valid, else return 0
 */
int checkRowValid(int row ,Game *game){
	int i=1;
	int value;
	int *arr = (int*)calloc(game->board_size+1,sizeof(int));
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
void freeGame(Game *game){
	int i;
	for(i=1;i<=game->board_size;i++){
		free(game->board[i]);
	}
	free(game->board);
	free(game);
}
