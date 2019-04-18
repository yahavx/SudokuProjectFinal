/*
 * Game.h:
 * This moudle implements some game operations: set, redo, undo, reset, autofill.
 * Also implements basic game checks, and supports interaction with the board.
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef GAME_H_
#define GAME_H_

#include "LinkedMovesList.h"

/* Defines game status */

typedef enum {
	INIT, EDIT, SOLVE
} Status;


/*
 * This struct represents a cell in the board.
 * 'value' contains the value of the cell, 0 if it is empty
 * 'fixed' = 1 iff the cell is fixed, i.e. can't be modified
 * 'hide' = 1 iff the cell value should not be printed with the board
 * 'blockStartI' is the row index of the first cell in the corresponding block
 * 'blockStartJ' is the column index of the first cell in the corresponding block
 */

typedef struct {
	int value;
	int fixed;
	int error;
} Cell;


/*
 * This struct represents a sudoku board.
 * 'board' is the matrix containing all the blocks of the board
 * n, m are the board dimensions, the board contains nxm blocks, each of size mxn
 */

typedef struct {
	int n, m;
	Cell *board;

} SudokuBoard;


/*
 * Sets cell (i,j) value to z. Mark cell as erroneous is needed, aswell for neighbours that were affected.
 * If cell is fixed and in solve mode, prints an error and returns 0 (otherwise 1).
 */
int set(SudokuBoard* sudoku, int i, int j, int z, Status mode, List* list);

/*
 * Performs an undo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Returns 1 on success, 0 otherwise.
 */
int undo(SudokuBoard* sudoku, List * list);

/*
 * Performs a redo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Returns 1 on success, 0 otherwise.
 */
int redo(SudokuBoard* sudoku, List * list);

/* Performs a reset operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Prints the board afterwards.
 */
int reset(SudokuBoard* sudoku, List * list);

/*
 * Fill all cells which contains a single legal value.
 * Returns 1 iff action is performed successfully.
 */
int autofill(SudokuBoard* sudoku, List *l);

#endif /* GAME_H_ */
