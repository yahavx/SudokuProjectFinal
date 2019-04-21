/*
 * LinkedMovesList.h:
 * This module implements a doubly-linked list.
 *
 * Used in order to perform board operations: undo, redo and reset.
 */

#ifndef LINKEDMOVESLIST_H_
#define LINKEDMOVESLIST_H_

/*
 * This struct represents a single move in the board, to support redo, undo and reset.
 *
 * If continueForward == 1, the next move should also be executed if a single redo opeartion
 * was performed on the relveant move (for autofill and generate), i.e. the move is part of
 * a sequence of moves that belong to the same redo/undo operation.  Same for continueBackward.
 */
typedef struct node {
	int oldCellValue;
	int newCellValue;
	int i;
	int j;
	int continueForward;
	int continueBackwards;
	struct node *next;
	struct node *prev;
} Node;

/*
 * This struct represents a doubly-linked list, to support redo, undo, and reset commands.
 *
 * Head and Tail represent the first and last moves added to the list respectively.
 * CurrentMove is the move that should be called first on the next redo/undo/reset operation.
 *
 * The head of the list is a sentinel (i.e. not an actual move, and identify-able).
 */
typedef struct {
	Node *Head;
	Node *Tail;
	Node *CurrentMove;
} List;

/*
 * Initializes a new list.
 */
List* createNewList();

/*
 * Adds a new move to the list (with function parameters as its values).
 * Any move beyond CurrentMove is removed.
 */
void addNewMove(List* l, int oldValue, int z, int i, int j, int forward,
		int backward);

/*
 * Resets a list, i.e. bring it to a state as if createNewList() was just called.
 * Free all memory resources of the previous list.
 */
void resetList(List* L);

/*
 * Destroys a list and free all its memory resources.
 */
void destroyList(List* L);


/* ======== TEST FUNCTIONS (remove before submitting) ============ */

void printNode(Node* n);
void printList(List* l);
void printList2(List* l);

#endif /* LINKEDMOVESLIST_H_ */
