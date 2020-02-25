/*
 * main.c
 *
 *  Created on: Feb 25, 2020
 *      Author: noada
 */
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "structs.h"
#include "SPBufferset.h"
int main(){
	Game *game=malloc(sizeof(Game));
	SP_BUFF_SET();
	game->num_of_columns_in_block=3;
	game->num_of_rows_in_block=4;
	game->board_size=12;
	intilizeEmptyBoard(game);
	printBoard(game);



	return 0;

}

