/*Stack struct*/

#ifndef STACK_H_
#define STACK_H_


#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

#include <stdlib.h>

#if !defined(__bool_true_false_are_defined) && !defined(__cplusplus)
typedef int bool;
#define true 1
#define false 0
#define __bool_true_false_are_defined
#endif

typedef struct Stack Stack;
typedef struct StackNode {
	int i;
	int j;
	int *arr_of_options;
	struct StackNode *next;
} StackNode;


Stack *stackCreate();
void stackDestroy(Stack *stack);
void stackClean(Stack *stack);
bool stackIsEmpty(Stack *stack);
size_t stackSize(Stack *stack);
StackNode *stackTop(Stack *stack);
bool stackPush(Stack *stack, int i, int j, int *arr_of_options);
StackNode *stackPop(Stack *stack);

#endif
#endif /* STACK_H_ */
