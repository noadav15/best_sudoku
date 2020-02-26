/*
 * solver.c
 * This module implements the backtracking algorithm.
 */


#include "structs.h"
#include <stdlib.h>
#include "game.h"
/*get an array and game board and update the array- array[i]==0 -> i is an valid number to the cell, else i is unvalid option
 *
 */
void fillArrWithOption(int *arr_of_options,Game *game,int row,int column){
	int i,j,value1=0,value2;
	int column_start_point;
	int row_start_point ;
	/*think about that tommorow!!!!!!!!!!!!!!!!!!!!!!!!*/
	if(column%game->num_of_columns_in_block==0){
		column_start_point= column -game->num_of_columns_in_block+1;
	}
	else{
		column_start_point= column -column%game->num_of_columns_in_block+1;
	}
	if(row%game->num_of_rows_in_block==0){
		row_start_point= row -game->num_of_rows_in_block+1;
	}
	else{
		row_start_point= row -row%game->num_of_rows_in_block+1;
	}
	for(i=1;i<=game->board_size;i++){
		if(game->board[i][column].value!=0){
			value1= game->board[i][column].value;
			arr_of_options[value1]++;
		}
		if(game->board[row][i].value!=0){
			value2= game->board[row][i].value;
			arr_of_options[value2]++;
		}
	}
	for(i=row_start_point;i<row_start_point+game->num_of_rows_in_block;i++){
		for(j=column_start_point;j<column_start_point+game->num_of_columns_in_block;j++){
			if(game->board[i][j].value!=0){
				value1=game->board[i][j].value;
				arr_of_options[value1]++;
			}
		}
	}
}
/*get the cell index and value and update the cell */
void setCellForAlgo(int row, int column, int value,Game *game){
	game->board[row][column].value=value;
}
/*get the algorithm and put a number in the current cell
 * return 1 if the cell is valid else return 0
 */
int fillCellByAlgo(int row, int column,Game *game, int *arr_of_options){
	int i=1;
	for(i=1;i<=game->board_size;i++){
		if(arr_of_options[i]==0){
			arr_of_options[i]=1;
			setCellForAlgo(row,column,i,game);
			return 1;
		}
	}
	return 0;
}
/*get array of option to fill a cell
 * return 1 if there are option available else return 0
 */
int stillHasOptionForCell(int *arr_of_options, Game *game){
	int i=1;
	for(i=1;i<=game->board_size;i++){
		if(arr_of_options[i]==0){
			return 1;
		}
	}
	return 0;
}
/*backtracking algorithm as we saw at class
 * return 1 if there a solution else return 0 */
int* backTracking(int i, int j,int start_index_row,int start_index_column, Game *game, int *arr_return_value){

	int succeed=0,*next_succeed, *return_value;
	int *arr_of_options=(int*)calloc(game->board_size+1,sizeof(int));
	/*if the cell already not empty continue to next*/
	if(game->board[i][j].value!=0){
		if(j<game->board_size){
			return_value=backTracking(i,j+1,start_index_row,start_index_column,game,arr_return_value);
			free(arr_of_options);
			return return_value;
		}
		else{
			if(i<game->board_size){
				return_value= backTracking(i+1,1,start_index_row,start_index_column,game,arr_return_value);
				free(arr_of_options);
				return return_value;
			}

			/*the last item not empty*/
			else{

				free(arr_of_options);
				arr_return_value[1]++;
				arr_return_value[0]=1;
				return arr_return_value;
			}
		}
	}
	/*should fill the cell*/

	else{
		printBoard(game);
		fillArrWithOption(arr_of_options,game,i,j);
		while(stillHasOptionForCell(arr_of_options,game)){
			succeed= fillCellByAlgo(i,j,game,arr_of_options);
			if(succeed){
				if(j<game->board_size){
					next_succeed= backTracking(i,j+1,start_index_row,start_index_column,game,arr_return_value);
				}
				else{
					if(i<game->board_size){
						next_succeed= backTracking(i+1,1,start_index_row,start_index_column,game,arr_return_value);
					}
					/*the last item not empty*/
					else{
						free(arr_of_options);
						arr_return_value[1]++;
						arr_return_value[0]=1;
						intilizeEmptyCell(&game->board[i][j]);
						return arr_return_value;
					}
				}
				if(next_succeed[0]==1){
					arr_return_value[0]=1;
					intilizeEmptyCell(&game->board[i][j]);
					/*if(i==start_index_column && j== start_index_column){*/
					arr_of_options[game->board[i][j].value]++;

					/*}
					else{
						free(arr_of_options);
						return arr_return_value;
					}*/

				}
			}
		}

		/*no more option*/
		game->board[i][j].value=0;
		free(arr_of_options);
		arr_return_value[0]=0;
		return arr_return_value;

	}
}
/*algorithem Deterministic
 * get board and return 1 if there a solution to the board and update the board
 * if there no solution return 0
 */
int fillTheBoard(Game *game){
	int i=1,j=1, start_index_row, start_index_column, check=0;
	int *arr_return_value= (int*)calloc(2,sizeof(int));
	for(i=1;i<=game->board_size && check ==0;i++){
		for(j=1;j<=game->board_size && check ==0;j++){
			if(game->board[i][j].value==0){
				if(check==0){
					start_index_row=i;
					start_index_column=j;
					check=1;
				}
			}
		}
	}

	arr_return_value= backTracking(start_index_row,start_index_column,start_index_row,start_index_column,game, arr_return_value);
	check=arr_return_value[1];
	free(arr_return_value);
	return check;

}
