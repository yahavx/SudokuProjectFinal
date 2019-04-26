/*
 * Stack.h:
 * This module implements a stack (FIFO).
 *
 * Used to simulate recursion.
 */

#ifndef STACK_H_
#define STACK_H_

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
 * This struct represents the list of parameters needed for a recursive call,
 * in order to find number of solutions to a board (i.e. supports exhaustive backtracking).
 */
typedef struct{
	int i;
	int j;
	int lastUsed;
} Parameters;

/*
 * This struct represents a Stack Node.
 */
typedef struct stackNode{
	Parameters params;
	struct stackNode *next;
} StackNode;

/*
 * This struct represent a stack.
 */
typedef struct {
	StackNode *top;
} Stack;


/*
 * Initializes an empty stack.
 */
Stack* initialize();

/*
 * Pushes a Parameters object (created from function parameters) to the stack.
 */
void push(int i, int j, int lastUsed, Stack *stk);

/*
 * Removes the top from the stack.
 * @pre - stack is not empty.
 */
Parameters pop(Stack *stk);

/*
 * Removes the top from the stack, inserts it's fields to the references of i, j, lastUsed.
 * @pre - stack is not empty.
 */
void popAndUpdate(Stack *stk, int *i, int *j, int *lastUsed);

/*
 * Returns the element at the top of the stack.
 * @pre - stack is not empty.
 */
Parameters top(Stack *stk);

/*
 * Removes stack and all it's elements, frees all memory resources.
 */
void destroyStack(Stack *stk);

/*
 * Returns 1 iff the stack is empty.
 */
int isEmpty(Stack *stk);


#endif /* STACK_H_ */
