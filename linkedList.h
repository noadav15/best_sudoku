/*
 * linkedList.h
 *
 *  Created on: 26 ????? 2020
 *      Author: Tal
 */

#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include <stdio.h>
#include <stdlib.h>

typedef struct _Move{
	int row;
	int column;
	int invalid;
	int value;
	int fixed;
	struct _Move *next;
	struct _Move *prev;
	int move_start;
	int edge;
} Move;

typedef struct MoveList{
		Move *cur_move;
} MoveList;

MoveList* createList();
void clearList(MoveList *list);
void clearListUndone(MoveList *list);
void insertToList(MoveList *list, int row, int column, int invalid, int value, int fixed, int move_start);
Move* undo(MoveList *list);
Move* redo(MoveList *list);


#endif /* LINKEDLIST_H_ */
