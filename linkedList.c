/*
 * linkedList.c
 *
 *  Created on: 25 ????? 2020
 *      Author: Tal
 */
#include <stdio.h>
#include <stdlib.h>
#include "linkedList.h"

Move* createEdge(){
	Move *move = (Move*)malloc(sizeof(Move));
	move->edge = 1;
	move->next = NULL;
	move->prev = NULL;
	return move;
}
MoveList* createList(){
	MoveList *list = NULL;
	list = (MoveList*)malloc(sizeof(MoveList));
	list->cur_move = createEdge();
	return list;
}

Move* createMove(int row, int column, int invalid, int value, int fixed, int move_start){
	Move *move = (Move*)malloc(sizeof(Move));
	move->column = column;
	move->row = row;
	move->fixed = fixed;
	move->move_start = move_start;
	move->invalid = invalid;
	move->value = value;
	move->next = NULL;
	move->prev = NULL;
	move->edge = 0;
	return move;
}
void clearList(MoveList *list){
	Move *temp = list->cur_move;
	if(temp == NULL){
		list->cur_move = NULL;
		return;
	}
	while(temp->prev != NULL){
		temp = temp->prev;
	}
	while(temp->next != NULL){
		temp = temp->next;
		free(temp->prev);
	}
	free(temp);
	list->cur_move = createEdge();
}

void clearListUndone(MoveList *list){
	Move *temp = list->cur_move;
	while(temp->next != NULL){
		temp = temp->next;
	}
	while(temp != list->cur_move){
		temp = temp->prev;
		free(temp->next);
	}
	if(temp->next != NULL){
		free(temp->next);
	}

}
void insert(MoveList *list, int row, int column, int invalid, int value, int fixed, int move_start){
	Move *move = createMove(row, column, invalid, value, fixed, move_start);
	clearListUndone(list);
	list->cur_move->next = move;
	move->prev = list->cur_move;
}

Move* undo(MoveList *list){
	Move* temp;
	if(list->cur_move->edge){
		return NULL;
	}
	temp = list->cur_move;
	list->cur_move = temp->prev;
	return temp;
}

Move* redo(MoveList *list){
	if(list->cur_move->next == NULL){
		return NULL;
	}
	list->cur_move = list->cur_move->next;
	return list->cur_move;
}

