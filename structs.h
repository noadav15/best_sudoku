/*
 * structs.h
 * this module contains all the structs that are needed for the game.
 */

#ifndef STRUCTS_H_
#define STRUCTS_H_
#include "linkedList.h"
enum status{Init, Edit, Solve};

typedef struct{
	int invalid;
	int value;
	int fixed;
	} Cell;

typedef struct{
	int num_of_rows_in_block;
	int num_of_columns_in_block;
	int board_size;
	int mark_errors;
	enum status cur_status;
	MoveList *move_history;
	Cell **board;
	} Game;

#endif /* STRUCTS_H_ */
