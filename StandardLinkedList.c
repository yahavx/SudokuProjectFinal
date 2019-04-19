/*
 * StandardLinkedList.c
 *
 *  Created on: 18 באפר׳ 2019
 *      Author: yahav
 */

#include "StandardLinkedList.h"
#include <stdlib.h>
#include "MainAux.h"

StandardNode* createNewStandardNode(int val, int varIndex);

void destroyAllStandardNodes(StandardNode* head);

/* =============== PUBLIC FUNCTIONS =============== */

StandardList* createNewStandardList() {
	StandardList* list = (StandardList*) malloc(sizeof(StandardList));
	assertMalloc((void*) list);
	list->Head = NULL;
	list->Tail = NULL;
	return list;

}

void addNewStandardMove(StandardList* list, int val, int varIndex) {
	StandardNode *move = createNewStandardNode(val, varIndex);
	if (list->Head == NULL) {
		list->Head = move;
		list->Tail = move;
	} else {
		list->Tail->next = move;
		list->Tail = move;
	}
}

int lookupInStandardList(StandardList* L, int val) {
	StandardNode *q = L->Head;
	while (q != NULL) {
		if (q->val == val) {
			return q->varIndex;
		}
		q = q->next;
	}
	return -2;
}

void destroyStandardList(StandardList* list) {
	if (list == NULL){
		return;
	}
	destroyAllStandardNodes(list->Head);
	free(list);
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Creates a new StandardNode.
 */
StandardNode* createNewStandardNode(int val, int varIndex) {
	StandardNode* newNode = (StandardNode*) malloc(sizeof(StandardNode));
	assertMalloc((void *) newNode);
	newNode->val = val;
	newNode->varIndex = varIndex;
	newNode->next = NULL;
	return newNode;
}

/*
 * This function destroy recursively all the nodes in a list, assuming
 * Head is the head of the list.
 */
void destroyAllStandardNodes(StandardNode* head) {
	if (head != NULL) {
		destroyAllStandardNodes(head->next);
		free(head);
	}
}

/* ======== TEST FUNCTIONS (remove before submitting) ============ */

void printStandardList(StandardList* list) {
	StandardNode *q = list->Head;
	while (q != NULL) {
		printf("(%d,%d) --> ", q->val, q->varIndex);
		q = q->next;
	}
	printf("NULL\n\n");
}
