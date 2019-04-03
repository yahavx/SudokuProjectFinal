/*
 * LinkedMovesList.c
 *
 *  Created on: Feb 22, 2019
 *      Author: Orin
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "LinkedMovesList.h"
#include "MainAux.h"

Node* createNewNode(int oldCellValue, int newCellValue, int i, int j, int forward, int backward) {
	Node* newNode = (Node*) malloc(sizeof(Node));
	assertMalloc((void *)newNode);
	newNode->oldCellValue = oldCellValue; /*the value in this cell before making the move */
	newNode->newCellValue = newCellValue;
	newNode->i = i;
	newNode->j = j;
	newNode->continueForward = forward;
	newNode->continueBackwards = backward;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

void destroyNode(Node* node) {
	free(node);
}

List* createNewList() {
	Node* Head = createNewNode(-1, -1, -1, -1, -1, -1); /* sentinel */
	List* list = (List*) malloc(sizeof(List));
	assertMalloc((void*)list);
	list->Head = Head;
	list->Tail = Head;
	list->Head->next = NULL;
	list->Head->prev = NULL;
	list->CurrentMove = (list->Head);
	return list;

}
void destroyAllNodes(Node* head) {
	if (head != NULL) {
		destroyAllNodes(head->next);
		destroyNode(head);
	}
}

void resetList(List* L){
	if (L->Tail == L->Head){ /* list is empty */
		return;
	}

	destroyFromCurrent(L->Head);
	L->Head->next = NULL;
	L->Tail = L->Head;
	L->CurrentMove = L->Head;

}

void destroyFromCurrent(Node* current) {
	destroyAllNodes(current->next);
}

void destroyList(List* L) {
	if (L == NULL) {
		printf("Warning! tried to destroy an uninitialized list.\n");
		return;
	}
	destroyAllNodes(L->Head);
	free(L);
}

void addNewMove(List* l, int oldValue, int z, int i, int j, int forward, int backward) {
	Node *move = createNewNode(oldValue, z, i, j, forward, backward);
	if (l->Head == l->Tail) {
		l->Tail = move;
		l->Tail->next = NULL;
		l->Tail->prev = l->Head;
		l->Head->next = l->Tail;
		l->CurrentMove = l->Tail;
	} else {
		destroyFromCurrent(l->CurrentMove);
		move->prev = l->CurrentMove;
		l->CurrentMove->next = move;
		l->CurrentMove = move;
		l->Tail = move;
	}
}

void printNode(Node* n) {
	printf("index: (%d,%d)", n->i, n->j);
	printf("old value: %d, ", n->oldCellValue);
	printf("new value: %d\n", n->newCellValue);

}

void printList2(List* l) {
	Node *q = (l->Head);
	while (q != NULL) {
		if (q == l->Head) {
			printf("(head)");
		}
		if (q == l->CurrentMove) {
			printf("(current)");
		}
		if (q == l->Tail) {
			printf("(tail)");
		}
		printf("index: (%d,%d), old val: %d, new val: %d ",q->i, q->j, q->oldCellValue, q->newCellValue);
		printf("forward = %d, backwards = %d",q->continueForward, q->continueBackwards);
		printf("\n--> \n");
		q = q->next;
	}
	printf("NULL\n\n");
}
void printList(List* l) {
	int k;
	Node* q = (l->Head);
	k = 0;
	printf("print all  list of moves done:\n");
	while (q != NULL) {
		k++;
		/*		printf("Node num : %d\n",k);*/
		if (q == (l->CurrentMove)) {
			printf(" this is the current move: ");
		}
		if (q == (l->Head)) {
			printf(" this is the head of the list: ");
		}
		if (q == (l->Tail)) {
			printf(" this is ths tail of the list: ");
		}
		printNode(q);
		q = q->next;
	}
}
