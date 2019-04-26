/*
 * test.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#include "Stack.h"

#ifndef TEST_H_
#define TEST_H_

int test();

typedef struct yosi Y;

Y* createY();

void getY(Y *bla);

/* ======== Stack ============ */

void emptyStack(Stack *stk); /* prints and removes all elements in stack */

/* ======== StandardLinkedList ============ */

void printStandardList(StandardList* l);

/* ======== LinkedMostList ============ */

void printNode(Node* n);
void printList(List* l);
void printList2(List* l);


#endif /* TEST_H_ */
