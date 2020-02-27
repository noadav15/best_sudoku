
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "game.h"

int processCommand(Game *game, char command[3][1024], int command_length){
	return 1;
}
int getCommand(Game *game, char input[1024], char command[4][1024]){
	char* token;
	int command_index = 0;
	const char delim[] = " \t\r\n";
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
	return processCommand(game, command, command_index+1);
}
