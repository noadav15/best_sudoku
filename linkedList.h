
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
/* LINKEDLIST_H_ */
