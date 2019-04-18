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
	INVALID_X,
	INVALID_Y,
	INVALID_Z,
	MARK_ERRORS_INVALID_VALUE,
	GUESS_INVALID_VALUE,

	/* commands */
	TOO_MANY_PARAMS,
	NOT_ENOUGH_PARAMS,
	AVAILABLE_IN_EDIT_AND_SOLVE,
	AVAILABLE_IN_EDIT,
	AVAILABLE_IN_SOLVE,
	WRONG_RANGE_OF_PARAM_VALUE,/*need to add a print commAnd*/
	TOO_LONG,
	INVALID_COMMAND,

	/* system errors */
	FILE_UNHANDLED,
	WRONG_PATH,
	MALLOC_FAILED,
	FGET_FAILED,
	GUROBI_FAILED
} Error;

/*
 * Defines actions performed by the user.
 */
typedef enum {
	WIN,
	SOLVEABLE,
	UNSOLVEABLE,
	WELCOME,
	EXITING,
	ENTER_COMMAND,
	GAME_SAVED,
	NUM_OF_SOLUTIONS
} Instruction;

typedef enum {
	COMMAND_TOO_LONG,
	UNKNOWN_COMMAND, /* command does not exist */
	ILLEGALY_HANDLED_COMMAND, /* command is not used properly */
	EMPTY_COMMAND,
	SOLVE_COMMAND, /* commands start here */
	EDIT_WITH_FILE_NAME,
	EDIT_WITHOUT_FILE_NAME,
	MARK_ERRORS,
	PRINT_BOARD,
	SET,
	VALIDATE,
	GUESS,
	GENERATE,
	UNDO,
	REDO,
	SAVE,
	HINT,
	GUESS_HINT,
	NUM_SOLUTIONS,
	AUTOFILL,
	RESET,
	EXIT

} CommandType;

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
 * @param errorType - error type.
 * @param range - a valid range for a parameter.
 */
void printErrorWithRange(Error errorType, int start, int end);

/*
 * Prints an error message.
 * @param errorType - info about the relevant error
 */
void printError(Error errorType);

/*
 * Prints the correct format of a command.
 * @param command - command type.
 * @param range - a valid range for the command parameters.
 */
void printFormatWithRange(CommandType command, int range);

/*
 * Prints the correct format of a command.
 * @param command - command type.
 */
void printFormat(CommandType command);

/*
 * Hints the user.
 * @param num - a parameter relevant to the message.
 */
void printInstructionWithRange(Instruction instType, int num);

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
