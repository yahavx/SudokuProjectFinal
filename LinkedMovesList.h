/*
 * LinkedMovesList.h
 * In this moudul we will implement the doubley linked list of the userses moves in order to
 * use it as part of the parser's and game's modoules
 *  Created on: Feb 22, 2019
 *      Author: Orin
 */

#ifndef LINKEDMOVESLIST_H_
#define LINKEDMOVESLIST_H_

typedef struct node{
	int oldCellValue;
	int newCellValue;
	int i;
	int j;
	int continueForward;
	int continueBackwards;
	struct node* next;
	struct node* prev;
}Node;

/*
 * This struct represents a single move in the board, to support redo, undo and reset.
 * If continueForward == 1, the next move should also be executed if a single redo opeartion
 * was performed on the current move (for autofill and generate), i.e. the move is part of
 * a sequence of moves that belong to the same redo/undo operation.
 */
typedef struct{
	Node * Head;
	Node * Tail;
	Node * CurrentMove;
}List;

/*
 * This struct represents a doubly-linked list, to support redo, undo, and reset commands.
 * The first node in the list should be a sentinel (created using createNewList function).
 */

/*@params : The function gets the relevant paramenters needed to create new node :
 * oldCellValue, lineInBlock, ColumnInBlock,lineInBoard,ColumnInBoard .
 *@return :The function returnes a pointer to the new node that was created.
 */
Node* createNewNode(int oldCellValue, int newCellValue, int i, int j, int forward, int backward);

/*@params: pointer to node
 * the function destroy the node and free resources.
 */
void destroyNode(Node* Node);

/*@params : pointer to a node which will be the head of the list
 * @return : the function create new list and return a pointer to a list which it's head is the node was gotten as parameter.
 */
List* createNewList();

/*
 * Destroy all nodes from the current node to the end (excluding)
 */
void destroyFromCurrent(Node* current);

/*
 * Removes all current moves from the list.
 */
void resetList(List* L);

/* @params : a pointer to the head of the list
 * The function destroy recursivley all the nodes in this list (free all resources).
 * This is an auxiliary function to "DestroyList" fuction.
 */
void destroyAllNodes(Node* Head);

/* @params: a pointer to the list that should be destroyed.
 *  The function deeply destroy all nodes in list and free all used resurces.
 */
void destroyList(List* L);

/*@params: a pointer to a list, a pointer to a mode which represents the current move.
 *  The function add the new node to the list of moves as the current move.
 */
void addNewMove (List* l, int oldValue, int z, int i, int j, int forward, int backward);

/*@param : pointer to node
 * print the node values
 * This is help function for internal tests
 */
void printNode(Node* n);

/*@param : pointer to list
 * print the node values for all nodes in this list
 * This is help function for internal tests
 */
void printList(List* l);
void printList2(List* l);

#endif /* LINKEDMOVESLIST_H_ */
