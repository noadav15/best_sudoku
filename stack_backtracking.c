/*
 * stack_backtracking.c
 *
 *  Created on: Feb 26, 2020
 *      Author: noada
 */


#define STACK_MAX 100
#include <stdlib.h>
#include <stdio.h>

struct Stack {
    int     data[STACK_MAX];
    int     size;
};
typedef struct Stack Stack;


void Stack_Init(Stack *S)
{
    S->size = 0;
}

int Stack_Top(Stack *S)
{
    if (S->size == 0) {
        printf( "Error: stack empty\n");
        return -1;
    }

    return S->data[S->size-1];
}

void Stack_Push(Stack *S, int d)
{
    if (S->size < STACK_MAX)
        S->data[S->size++] = d;
    else
        printf("Error: stack full\n");
}

void Stack_Pop(Stack *S)
{
    if (S->size == 0)
        printf("Error: stack empty\n");
    else
        S->size--;
}

