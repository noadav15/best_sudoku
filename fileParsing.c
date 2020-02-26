/*
 * fileParsing.c
 *
 *  Created on: 26 ????? 2020
 *      Author: Tal
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h"
int readFromFile(Game *game, char* fileDir){
	FILE *fptr = fopen(fileDir, "r");
	if(fptr == NULL){
		printf("ERROR: invalid file name or file doesn't exist\n");
		return 0;
	}
	fclose(fptr);
	return 1;
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
	fprintf(fptr, converted);
	fprintf(fptr, " ");
	sprintf(converted, "%d", game->num_of_rows_in_block);
	fprintf(fptr, converted);
	fprintf(fptr, "\n");
	for(i=1;i<=game->board_size;i++){
		for(j=1;j<=game->board_size;j++){
				cur = (game->board)[i][j];
				sprintf(converted, "%d", cur.value);
				fprintf(fptr, converted);
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
