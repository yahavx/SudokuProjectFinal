/*
 * Stack.c:
 * This moudle implements Stack.h
 */

#include <stdio.h>
#include <stdlib.h>
#include "Stack.h"
#include "MainAux.h"

/* =============== PUBLIC FUNCTIONS =============== */

void push(int i, int j, int lastUsed, Stack *stk) {
	StackNode *p = malloc(sizeof(StackNode));
	assertMalloc((void*) p);
	p->params.i = i;
	p->params.j = j;
	p->params.lastUsed = lastUsed;
	p->next = stk->top;
	stk->top = p;
}

Stack* initialize() {
	Stack* stk = malloc(sizeof(Stack));
	assertMalloc((void*) stk);
	stk->top = NULL;
	return stk;
}

Parameters pop(Stack *stk) {
	Parameters params;
	StackNode *p = NULL;

	params.i = stk->top->params.i;
	params.j = stk->top->params.j;
	params.lastUsed = stk->top->params.lastUsed;
	p = stk->top;

	stk->top = stk->top->next;
	free(p);
	return params;
}



void destroyStack(Stack *stk) {
	while (stk->top != NULL) {
		pop(stk);
	}
	free(stk);
}

Parameters top(Stack *stk) {
	return stk->top->params;
}

void popAndUpdate(Stack *stk, int *i, int *j, int *lastUsed) {
	Parameters params = pop(stk);
	*i = params.i;
	*j = params.j;
	*lastUsed = params.lastUsed;
}

int isEmpty(Stack *stk) {
	return stk->top == NULL;
}


/* ======== TEST FUNCTIONS (remove before submitting) ============ */

void emptyStack(Stack *stk) {
	while (stk->top != NULL) {
		printf("(%d,%d),%d -> ", top(stk).i, top(stk).j, top(stk).lastUsed);
		pop(stk);
	}

	printf("NULL\n");
}
