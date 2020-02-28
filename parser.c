
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "solve.h"
#include "game.h"


void initInput(char input[1024]){
	int i;
	for(i = 0; i < 1024; i++){
		input[i] = -1;
	}
}

void initCommand(char command[4][1024]){
	int i;
	int j;
	for(j = 0; j < 4; j++){
		for(i = 0; i < 1024; i++){
			command[j][i] = -1;
		}
	}
}


Game* processCommand(Game *game, char command[4][1024], int command_length){
	Game *out_game;
	int num_of_sol, fill_ret;
	char *commandType = command[0];
	/*printf("command length: %d\n", command_length);*/
	if(!strcmp(commandType, "solve")){
		if(command_length == 1){
			printf("ERROR: please enter a file name after solve\n");
			return game;
		}
		if(command_length > 2){
			printf("ERROR: too many parameters for solve\n");
			return game;
		}
		out_game = solve(command[1]);
	}
	else if(!strcmp(commandType, "edit")){
		if(command_length > 2){
				printf("ERROR: too many parameters for edit\n");
				return game;
		}
		if(command_length == 1){
			out_game = edit("");
		}
		else{
			out_game = edit(command[1]);
		}
	}
	else if(!strcmp(commandType, "mark_errors") && game_status == Solve){
		if(command_length > 2){
			printf("ERROR: too many parameters for mark_errors\n");
			return game;
		}
		if(command_length == 1){
			printf("ERROR: please enter 1 or 0  after mark_errors\n");
			return game;
		}
		if(strcmp(command[1], "1") && strcmp(command[1], "0")){
			printf("ERROR: mark_errors parameter should be 1 or 0\n");
			return game;
		}
		else{
			if(!strcmp(command[1], "1")){
				markErrors(1);
				printf("mark_errors: %d\n", mark_errors);
			}
			else{
				markErrors(0);
			}
			printBoard(game);
			return game;
		}
	}
	else if(!strcmp(commandType, "print_board")){
		if(command_length > 1){
			printf("ERROR: print_board command doesn't have parameters\n");
			return game;
		}
		else{
			if(game_status != Solve){
				printf("ERROR: print_board command is only available in Solve mode\n");
				return game;
			}
			printBoard(game);
			return game;
		}

	}
	else if(!strcmp(commandType, "num_solutions")){
		if(command_length > 1){
					printf("ERROR: print_board command doesn't have parameters\n");
					return game;
				}
				else{
					if(game_status != Solve || game_status != Edit){
						printf("ERROR: print_board command is only available in Solve or Edit mode\n");
						return game;
					}
					 num_of_sol = countSolutions(game);
					 printf("The number of possible solutions to this board is: %d\n", num_of_sol);
					 return game;
				}
	}
	else if(!strcmp(commandType, "autofill")){
		if(command_length > 1){
				printf("ERROR: autofill command doesn't have parameters\n");
				return game;
		}
		if(game_status != Solve){
				printf("ERROR: autofill command is only available in Solve mode\n");
				return game;
		}
		fill_ret = autoFillBoard(game);
		if(fill_ret){
			printBoard(game);
		}
		return game;
	}
	else if(!strcmp(commandType, "exit")){
			if(command_length > 1){
					printf("ERROR: exit command doesn't have parameters\n");
					return game;
			}
			exitGame(game);
	}
	else if(!strcmp(commandType, "reset")){
			if(command_length > 1){
					printf("ERROR: reset command doesn't have parameters\n");
					return game;
			}
			if(game_status != Solve || game_status != Edit){
					printf("ERROR: reset command is only available in Solve or Edit mode\n");
					return game;
			}
			resetBoard(game);
	}
	if(out_game != NULL){
		printBoard(out_game);
		return out_game;
	}
	return game;
}

Game* getCommand(Game *game, char input[1024], char command[4][1024]){
	char* token;
	int command_index = 0;
	const char delim[] = " \t\r\n";
	initInput(input);
	initCommand(command);
	fflush(stdin);
	if(feof(stdin)){
		exitGame(game);
	}
	printf("enter command:\n");
	fgets(input, 1024, stdin);
	if(strlen(input) > 256){
		printf("input is too long:\n");
		return 0;
	}
	token = strtok(input, delim);
	while(token != NULL){
		if(command_index <= 3){
			strcpy(command[command_index], token);
		}
		command_index++;
		token = strtok(NULL, delim);
	}
	return processCommand(game, command, command_index);
}

