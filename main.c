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
#include "linkedList.h"
int main(){
	MoveList *list = createList();
	Move *cur;
		insertToList(list, 10, 11, 0, 13, 1, 1);
		insertToList(list, 10, 11, 0, 10, 1, 0);
		insertToList(list, 10, 11, 0, 11, 1, 0);
		insertToList(list, 10, 11, 0, 20, 1, 0);
		insertToList(list, 10, 11, 0, 34, 1, 1);
		insertToList(list, 10, 11, 0, 12, 1, 1);
		insertToList(list, 10, 11, 0, 7, 1, 1);
		insertToList(list, 10, 11, 0, 3, 1, 0);
		insertToList(list, 10, 11, 0, 2, 1, 1);
		insertToList(list, 10, 11, 0, 1, 1, 0);
		cur = undo(list);
		printf("%d\n", cur->value);
		cur = undo(list);
		printf("%d\n", cur->value);
		cur = redo(list);
		printf("%d\n", cur->value);
		cur = redo(list);
		printf("%d\n", cur->value);
		cur = redo(list);
		if(cur == NULL){
			printf("good");
		}
		clearList(list);
		printf("done");
		return 0;



}

