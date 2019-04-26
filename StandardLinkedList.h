/*
 * StandardLinkedList.h
 * This module implements a standard linked list, with int nodes.
 */

#ifndef STANDARDLINKEDLIST_H_
#define STANDARDLINKEDLIST_H_

/*
 * This struct represents a single move in the board, to support redo, undo and reset.
 * If continueForward == 1, the next move should also be executed if a single redo opeartion
 * was performed on the relveant move (for autofill and generate), i.e. the move is part of
 * a sequence of moves that belong to the same redo/undo operation.  Same for continueBackward.
 */
typedef struct snode {
	int val;
	int varIndex;
	struct snode *next;
} StandardNode;

/*
 * This struct represents a doubly-linked list, to support redo, undo, and reset commands.
 * Head is the first node in the list.
 */
typedef struct {
	StandardNode *Head;
	StandardNode *Tail;
} StandardList;

/*
 * Initializes a new list.
 */
StandardList* createNewStandardList();

/*
 * Adds a new move to the list (with function parameters as its values).
 * Any move beyond CurrentMove is removed.
 */
void addNewStandardMove(StandardList* l, int val, int varIndex);

/*
 * Searchs for val in the list.
 * If val is found in a StandardNode, returns it's varIndex. Otherwise, returns -2.
 */
int lookupInStandardList(StandardList* L, int val);

/*
 * Destroys a list and free all its memory resources.
 */
void destroyStandardList(StandardList* L);

#endif /* STANDARDLINKEDLIST_H_ */
