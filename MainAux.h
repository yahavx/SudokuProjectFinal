/*
 * MainAux.h:
 * This module is in an auxiliary for the main. It supports basic sudoku board handling (creating, destroying, etc).
 *
 * Interaction with the user is done through this module.
 *
 * In addition, it defines commands, errors and instruction enums.
 */

#ifndef MAINAUX_H_
#define MAINAUX_H_

#include "Game.h"

/* Defines error type */
typedef enum {
	/* Commands unable to execute */
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
	NOT_ENOUGH_EMPTY_CELLS,
	UNABLE_TO_VERIFY,
	PUZZLE_GENERATOR_FAILED,

	/* Invalid commands or commands with invalid parameters (i.e. invalid input) */
	TOO_MANY_PARAMS,
	NOT_ENOUGH_PARAMS,
	AVAILABLE_IN_EDIT_AND_SOLVE,
	AVAILABLE_IN_EDIT,
	AVAILABLE_IN_SOLVE,
	TOO_LONG,
	WRONG_PATH,
	INVALID_COMMAND,

	/* System errors */
	FILE_UNHANDLED,
	MALLOC_FAILED,
	FGET_FAILED,
	GUROBI_FAILED
} Error;

/*
 * Defines actions performed by the user and general game states.
 */
typedef enum {
	WIN,
	NO_WIN, /* The board is full, but the solution is incorrect */
	FAKE_WIN, /* Happens when user loads an already solved board in solve mode */
	LOAD_PUZZLE,
	SOLVEABLE,
	UNSOLVEABLE,
	WELCOME,
	EXITING,
	ENTER_COMMAND,
	GAME_SAVED,
	NUM_OF_SOLUTIONS,
	MARK_ERRORS_SET,
	NO_CHANGE
} Instruction;

/*
 * Defines game commands and invalid commands.
 */
typedef enum {

	/* Illegal comands */
	COMMAND_TOO_LONG,
	ILLEGALY_HANDLED_COMMAND,
	EMPTY_COMMAND,

	/* Legal commands */
	SOLVE_COMMAND,
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
 * Checks if  memory allocation by malloc was failed. Terminates the program and prints a message if indeed.
 * @param pointer - the pointer which a malloc operation was perfromed on
 */
void assertMalloc(void* pointer);

/*
 * Creates a new sudokuBoard, allocates space to all fields, and set all values to zero.
 */
SudokuBoard* initializeBoard(int n, int m);

/*
 * Destroy a sudoku board, free all its memory resources.
 */
void destroyBoard(SudokuBoard* sudoku);

/*
 * Performs deep clone of a SudokuBoard sturct.
 * Returns a pointer to the clone.
 */
SudokuBoard* clone(SudokuBoard* sudoku);

/*
 * Copy cell values (only) from sudoku to sudokuCopy.
 */
void copy(SudokuBoard* sudoku, SudokuBoard* sudokuCopy);

/**** Print functions ****/

/*
 * Prints a sudoku board.
 * @param mode - game mode
 * @param markErrors - the value of the mark errors variable.
 */
void printBoard(SudokuBoard *sudoku, Status mode, int markErrors);

/*
 * Prints a seperator row.
 * @param length - row length.
 */
void printSeperator(int length);

/*
 * Informs the user about a cell value update.
 * @params: i - row index, j - column index (0 based), oldVal - old cell value, newVal - newly assigned value.
 * @pre - command is AUTOFILL, GUESS, REDO, UNDO (i.e. commands that change cells without the user implicitly choosing them, in solve mode).
 */
void printCellUpdate(CommandType command, int i, int j, int oldVal, int newVal);

/*
 * Prints an error message.
 * @param errorType - error type.
 * @params start, end - a valid range for the parameter. If the error should not describe a range (i.e. generate), uses
 * first parameter only (start).
 * @pre - errorType is a parameter-type error.
 */
void printInvalidParameter(Error errorType, int start, int end);

/*
 * Prints an error message.
 * @param errorType - error type.
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
 * Prints a guess hint message about a single value.
 * @params val - cell value, score - score of that value (found by LP).
 */
void printGuessHintScore(int val, float score);

/*
 * Used for mark_errors, num_solutions and hint.
 * @param param - mark_errors new value, number of solutions to the board, or the value found by hint.
 */
void printInstructionWithParam(CommandType command, int param);

/*
 * Prints general messages about the game state.
 */
void printInstruction(Instruction instType);

/*
 * Returns 1 if the current board is solved, i.e. in solve mode
 * and all the cells are filled and not erroneous.
 *
 * If the board is full, prints appropriate message (win, or error).
 */

/*
 * Prints 'wrong mode' message when trying to execute a command in an illegal mode.
 * @params: errorType - which modes it is available in.
 *          mode - current game mode.
 */
void printWrongGameMode(Error errorType, Status mode);

/*
 * Prints current game status.
 */
void printCurrentStatus(Status mode);

/*
 * Checks if a game is finished, i.e. board is solved and on solve mode. Returns 1 iff yes.
 *
 * Also prints an appropriate message if the board is full (win/error), if on solve mode.
 */
int validateSolution(SudokuBoard* sudoku, Status mode);

/*
 * Returns 1 iff the board is solved.
 */
int isSolved(SudokuBoard *sudoku);

/*
 * Returns 1 iff the board is full (i.e. all cells are filled with a value from 1 to N).
 */
int isFull(SudokuBoard *sudoku);

#endif /* MAINAUX_H_ */
