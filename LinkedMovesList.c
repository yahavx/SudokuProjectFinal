/*
 * LinkedMovesList.c:
 * This module implements LinkedMovesList.h.
 */

#include <stdlib.h>
#include "LinkedMovesList.h"
#include "MainAux.h"

Node* createNewNode(int oldCellValue, int newCellValue, int i, int j,
		int forward, int backward);

void destroyFromCurrent(Node* current);

void destroyAllNodes(Node* Head);

/* =============== PUBLIC FUNCTIONS =============== */

List* createNewList() {
	Node* Head = createNewNode(-1, -1, -1, -1, -1, -1); /* Head is always a sentinel */
	List* list = (List*) malloc(sizeof(List));
	assertMalloc((void*) list);
	list->Head = Head;
	list->Tail = Head;
	list->Head->next = NULL;
	list->Head->prev = NULL;
	list->CurrentMove = (list->Head);
	return list;

}

void addNewMove(List* l, int oldValue, int z, int i, int j, int forward,
		int backward) {
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

void resetList(List* L) {
	if (L->Tail == L->Head) { /* list is empty */
		return;
	}

	destroyFromCurrent(L->Head);
	L->Head->next = NULL;
	L->Tail = L->Head;
	L->CurrentMove = L->Head;

}

void destroyList(List* L) {
	destroyAllNodes(L->Head);
	free(L);
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Creates a new Node.
 */
Node* createNewNode(int oldCellValue, int newCellValue, int i, int j,
		int forward, int backward) {
	Node* newNode = (Node*) malloc(sizeof(Node));
	assertMalloc((void *) newNode);
	newNode->oldCellValue = oldCellValue;
	newNode->newCellValue = newCellValue;
	newNode->i = i;
	newNode->j = j;
	newNode->continueForward = forward;
	newNode->continueBackwards = backward;
	newNode->next = NULL;
	newNode->prev = NULL;
	return newNode;
}

/*
 * This function destroys recursively all the nodes in a list, assuming
 * Head is the head of the list.
 */
void destroyAllNodes(Node* head) {
	if (head != NULL) {
		destroyAllNodes(head->next);
		free(head);
	}
}

/*
 * Destroy all nodes from the current node (excluding) to the end.
 */
void destroyFromCurrent(Node* current) {
	destroyAllNodes(current->next);
}
