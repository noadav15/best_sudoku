/*
 * stack.c
 *
 *  Created on: Feb 26, 2020
 *      Author: noada
 */


#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "solve.h"


struct Stack {
	size_t count; 	/** The number of items in the stack. **/
	StackNode *top;	/** The top item of the stack. **/
};

Stack *stackCreate()
{
	/* Create a stack and set everything to the default values. */
	Stack *stack = (Stack *) malloc(sizeof(stack));
	if(stack==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	stack->count = 0;
	stack->top = 0;

	return stack;
}

void stackDestroy(Stack *stack)
{
	stackClean(stack);
	free(stack);
}

void stackClean(Stack *stack){

	StackNode *node;
	while(stackSize(stack)>0){
		node = stackPop(stack);
		free(node->arr_of_options);
		free(node);
	}
}

size_t stackSize(Stack *stack)
{
	return stack->count;
}

StackNode *stackTop(Stack *stack)
{
	return stack->top;
}

bool stackPush(Stack *stack, int i, int j, int *arr_of_options)
{
	StackNode *newNode = (StackNode *) malloc(sizeof *newNode);
	if(newNode==NULL){
		printf("ERROR: problem with memory allocation\n");
		exit(0);
	}
	newNode->arr_of_options=arr_of_options;
	newNode->i=i;
	newNode->j=j;
	newNode->next = stack->top;
	stack->top = newNode;

	stack->count += 1;
	return true;
}
/*pre- call only if stack.size>0*/
StackNode *stackPop(Stack *stack)
{
	StackNode *oldTop;
	oldTop = stack->top;
	stack->top = oldTop->next;
	stack->count -= 1;
	return oldTop;
}

