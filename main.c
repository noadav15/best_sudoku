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
#include "fileParsing.h"
#include "parser.h"
#include "game.h"
int main(){
		/*Game *game;
		game_status = Solve;
		SP_BUFF_SET();
		game = initializeGame(2,2);
		setCell(1, 1, 1, game, 1);
		setCell(3, 2, 1, game, 1);
		printBoard(game);
		printf("%d\n", invalidCell(game, 2, 3, 0));*/

		char input[1024];
		char command[4][1024];
		Game *game = NULL;
		game_status = Init;
		SP_BUFF_SET();
		while(1){
			game = getCommand(game, input, command);
		}
		if(game){

		}
		/*Game *game;
		Game *game2;
		game_status = Solve;
		game = solve("test.txt");
		printBoard(game);
		setCell(1, 1, 0, game, 1);
		printBoard(game);
		undoMove(game);
		printBoard(game);
		setCell(2, 2, 3, game, 1);
		printBoard(game);
		undoMove(game);
		printBoard(game);
		redoMove(game);
		printBoard(game);
		saveToFile(game, "test.text");
		printf("done");
		Game *game =  readFromFile("test.txt", 1);
		if(game != NULL){
			printBoard(game);
		}*/
		freeGame(game);
		printf("done");
		return 0;



}

