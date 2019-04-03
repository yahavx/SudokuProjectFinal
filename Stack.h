/*
 * Stack.h
 *
 *  Created on: 24 áôáø× 2019
 *      Author: yahav
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct{
	int i;
	int j;
	int lastUsed;
} Parameters;

/* This struct represents the list of parameters needed for a recursive call, to find number of solutions to a board.
 */

typedef struct stackNode{
	Parameters params;
	struct stackNode *next;
} StackNode;

/* This struct represents a StackNode (for a recursive call).
 * Currently supports int only.
 */

typedef struct {
	StackNode *top;
} Stack;

/*
 * This struct represent a stack
 */

/*
 *  Push param to the stack.
 */
void push(int i, int j, int lastUsed, Stack *stk);

/*
 * Initialize an empty stack.
 */
Stack* initialize();

/*
 * Removes the head from the stack.
 * @pre - stack is not empty.
 */
Parameters pop(Stack *stk);

/*
 * Prints all elements (and removes them). for test only
 */
void emptyStack(Stack *stk);

/*
 * Returns the element at the top of the stack.
 * @pre - stack is not empty.
 */
Parameters top(Stack *stk);

/*
 * Removes stack and all it's elements, free all memory resources.
 */
void destroyStack(Stack *stk);

/*
 * Returns 1 iff the stack is empty.
 */
int isEmpty(Stack *stk);

/*
 * Removes the head from the stack, inserts it's fields to i, j, lastUsed.
 */
void popAndUpdate(Stack *stk, int *i, int *j, int *lastUsed);

#endif /* STACK_H_ */
