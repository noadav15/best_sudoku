/*
 * main.c
 *
 *  Created on: Feb 25, 2020
 *      Author: noada
 */
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "solve.h"
#include "structs.h"
#include "SPBufferset.h"
int main(){
	int count=0;
		Game *game=(Game*)malloc(sizeof(Game));
		SP_BUFF_SET();
		game->num_of_columns_in_block=2;
		game->num_of_rows_in_block=2;
		game->board_size=4;
		intilizeEmptyBoard(game);
		game->board[2][4].value=2;
		game->board[2][4].fixed=1;
		game->board[2][5].value=4;
		game->board[2][5].invalid=1;
		printBoard(game);

		count= fillTheBoard(game);
		printBoard(game);
		printf("%d=",count);
		freeGame(game);


		return 0;



}

