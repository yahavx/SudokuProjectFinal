/*
 * MainAux.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Game.h"
#include "SPBufferset.h"

/* Defines error type */
typedef enum {
	/* commands unable to execute */
	FIXED_CELL,
	ERRONEOUS_BOARD,
	NO_MOVE_TO_UNDO,
	NO_MOVE_TO_REDO,
	FILE_NOT_EXIST,
	INVALID_BOARD_FORMAT,
	UNSOLVEABLE_BOARD,
	CELL_IS_NOT_EMPTY,

	/* commands */
	TOO_MANY_PARAMS,
	NOT_ENOUGH_PARAMS,
	AVAILABLE_IN_EDIT_AND_SOLVE,
	AVAILABLE_IN_EDIT,
	AVAILABLE_IN_SOLVE,
	WRONG_RANGE_OF_PARAM_VALUE,/*need to add a print commAnd*/

	/* system errors */
	FILE_UNHANDLED,
	MALLOC_FAILED,
	FGET_FAILED,
	GUROBI_FAILED
} Error;

/*
 * Defines actions performed by the user.
 */
 typedef enum {
	 WIN, SOLVEABLE, UNSOLVEABLE, WELCOME, EXITING
 } Instruction;

/*
 * Creates a new sudokuBoard, allocate space to all fields, and set all values to zero.
 */
SudokuBoard* initializeBoard(int n, int m);

/*
 * Checks if malloc memory allocation failed. terminates program if indeed.
 * @param pointer - the pointer which a malloc operation was perfromed on
 */
void assertMalloc(void* pointer);

/*
 * Prints the current board state.
 */
void printBoard(SudokuBoard *sudoku, Status mode, int markErrors);
void printBoard3(SudokuBoard *sudoku);

/*
 * Prints a seperator
 */
void printSeperator(int length);

/*
 * Sets cell fields to zero, and value to 'data'.
 */
void initCell(Cell *c, int data);

/*
 * Prints an error message.
 * @param errorType - info about the relevant error
 */
void printError(Error errorType);

/*
 * Hints the user.
 */
void printInstruction(Instruction instType);

/*
 * Initialize the array by setting all elements to zero.
 * @param c - pointer to the array of char, N - array size
 */
void initializeArray(char c[], int N);

/*
 * Free the cells array of the block. does not free the block struct itself
 */
void destroyBlock(Block* block, int m);

/*
 * Destroy a sudoku board, free all memory resources.
 */
void destroyBoard(SudokuBoard* sudoku);

/*
 * Returns a deep clone of sudoku.
 */
SudokuBoard* clone(SudokuBoard* sudoku);

/*
 * Copy values (only) from sudoku to sudokuCopy.
 */
void copy(SudokuBoard* sudoku, SudokuBoard* sudokuCopy);


#endif /* MAINAUX_H_ */
