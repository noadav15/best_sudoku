
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "structs.h"
#include "game.h"

int checkFormatNotFixed(char* token){
	if(strlen(token) == 1){
		if(isdigit(token[0])){
			return 1;
		}
	}
	return 0;
}
int checkFormatFixed(char* token){
	if(strlen(token) == 2){
				if(isdigit(token[0]) && token[1] == '.'){
						return 1;
				}
			}
	return 0;
}

int checkMN(char *token){
	if(token == NULL){
				printf("ERROR: invalid format\n");
				return 0;
		}
		if(!checkFormatNotFixed(token)){
				printf("ERROR: invalid format\n");
				return 0;
		}
	return 1;
}


Game* readFromFile(char* fileDir, int check_errors){
	int num_of_columns_in_block, num_of_rows_in_block, board_size;
	long file_size;
	char *buffer;
	char *token;
	FILE *fptr;
	int x = 1;
	int y = 1;
	Game *game = (Game*)malloc(sizeof(Game));
	char delim[] = " \t\r\n\v\f";
	fptr = fopen(fileDir, "r");
	if(fptr == NULL){
		printf("ERROR: invalid file name or file doesn't exist\n");
		free(game);
		return NULL;
	}
	fseek(fptr, 0, SEEK_END);
	file_size = ftell(fptr);
	fseek(fptr, 0, SEEK_SET);
	buffer = (char*)malloc(file_size);
	if(buffer == NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
		return NULL;
	}
	fread (buffer, 1, file_size, fptr);
	token = strtok(buffer, delim);
	if(checkMN(token) == 0){
		return NULL;
	}
	num_of_columns_in_block = token[0] - '0';
	token = strtok(NULL, delim);
	if(checkMN(token) == 0){
			return NULL;
	}
	num_of_rows_in_block = token[0] -'0';
	board_size = num_of_columns_in_block * num_of_rows_in_block;
	game->board_size = board_size;
	game->num_of_columns_in_block = num_of_columns_in_block;
	game->num_of_rows_in_block = num_of_rows_in_block;
	game->move_history = createList();
	intilizeEmptyBoard(game);
	token = strtok(NULL, delim);
	while( token != NULL ) {
	      if(checkFormatNotFixed(token)){
	    	  (game->board)[y][x].value = token[0] - '0';
	    	  (game->board)[y][x].fixed = 0;
	      }
	      else if(checkFormatFixed(token)){
	    	  (game->board)[y][x].value = token[0] - '0';
	    	  (game->board)[y][x].fixed = 1;
	      }
	      else{
	    	  goto free;
	      }
	      token = strtok(NULL, delim);
	      	    if(token == NULL && (x != board_size || y != board_size)){
	      	    	  goto free;
	      }
	      if(x == board_size){
	    	  if(y == board_size){
	    		  break;
	    	  }
	    	  x = 1;
	    	  y++;
	      }
	      else{
	    	  x++;
	      }

	}
	token = strtok(NULL, delim);
	if(check_errors == 1 && checkFixedCells(game) == 0){
		goto free;
	}

	if(x != board_size || y != board_size || token != NULL){
		free:
		printf("ERROR: invalid format\n");
		freeGame(game);
		free(buffer);
		fclose(fptr);
		return NULL;
	}
	free(buffer);
	fclose(fptr);
	return game;
}

void saveToFile(Game *game, char* fileDir){
	int i, j;
	Cell cur;
	char converted[12];
	FILE *fptr = fopen(fileDir, "w");
	if(fptr == NULL){
				printf("ERROR: invalid file name or file doesn't exist\n");
				return;
	}

	sprintf(converted, "%d", game->num_of_columns_in_block);
	fprintf(fptr, "%s", converted);
	fprintf(fptr, " ");
	sprintf(converted, "%d", game->num_of_rows_in_block);
	fprintf(fptr,"%s", converted);
	fprintf(fptr, "\n");
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
				cur = (game->board)[i][j];
				sprintf(converted, "%d", cur.value);
				fprintf(fptr, "%s", converted);
				if(cur.fixed){
					fprintf(fptr, ".");
				}
				if(j == game->board_size){
					fprintf(fptr, "\n");
				}
				else{
					fprintf(fptr, " ");
				}
			}
	}
	fclose(fptr);
}

