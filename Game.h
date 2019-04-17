/*
 * Game.h:
 * This moudle implements all game operations.
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef GAME_H_
#define GAME_H_

#include "LinkedMovesList.h"

typedef enum {
	INIT, EDIT, SOLVE
} Status;

/* Defines game status */

typedef struct {
	int value;
	int fixed;
	int error;
} Cell;

/*
 * This struct represents a cell in the board.
 *  'value' contains the value of the cell, 0 if it is empty
 *  'fixed' = 1 iff the cell is fixed, i.e. can't be modified
 *  'hide' = 1 iff the cell value should not be printed with the board
 *  'blockStartI' is the row index of the first cell in the corresponding block
 *  'blockStartJ' is the column index of the first cell in the corresponding block
 */


typedef struct {
	int n, m;
	Cell *board;

} SudokuBoard;

/* This struct represents a sudoku board.
 * 'board' is the matrix containing all the blocks of the board
 * n, m are the board dimensions, the board contains nxm blocks, each of size mxn
 * 'isValid' = 1 if the matrix is solveable. this field is updated upon calling 'validate'
 */


/*
 * Sets cell (i,j) value to z, and mark an error if needed.
 * If cell is fixed and in solve mode, prints an error.
 */
int set(SudokuBoard* sudoku, int i, int j, int z, Status mode, List* list);

/*
 * Performs an undo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board
 * Returns 1 on success, 0 otherwise.
 */
int undo(SudokuBoard* sudoku, List * list);

/*
 * Performs a redo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Returns 1 on success, 0 otherwise.
 */
int redo(SudokuBoard* sudoku, List * list);

/* Implementation of reset command
 * @params: pointer to undo/redo list and pointer to the board.
 * Prints the board afterwards.
 */
int reset(SudokuBoard* sudoku, List * list);

/*
 * Fill all cells which contains a single legal value. Returns 1 iff action is performed.
 */
int autofill(SudokuBoard* sudoku, List *l);



#endif /* GAME_H_ */
