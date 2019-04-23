/*
 * MainAux.c:
 * This module implements MainAux.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "Parser.h"
#include "MainAux.h"
#include "GameAux.h"

void initCell(Cell *c, int data);

int isFull(SudokuBoard *sudoku);

/* =============== PUBLIC FUNCTIONS =============== */

void assertMalloc(void* pointer) {
	if (pointer == NULL) {
		printError(MALLOC_FAILED);
		exit(0);
	}
}

SudokuBoard* initializeBoard(int n, int m) {
	int i, N;
	SudokuBoard *sudoku = (SudokuBoard*) malloc(sizeof(SudokuBoard));
	assertMalloc((void*) (sudoku));
	sudoku->n = n, sudoku->m = m;

	N = n * m;
	sudoku->board = (Cell*) malloc(N * N * sizeof(Cell));
	assertMalloc((void*) (sudoku->board));

	for (i = 0; i < N * N; i++) {
		initCell(&(sudoku->board[i]), 0);
	}
	return sudoku;

}

void destroyBoard(SudokuBoard* sudoku) {
	if (sudoku != NULL) {
		free(sudoku->board);
		free(sudoku);
		sudoku = NULL;
	}

}

SudokuBoard* clone(SudokuBoard* sudoku) {
	SudokuBoard* sudokuCopy;

	int i, j, n = sudoku->n, m = sudoku->m;

	sudokuCopy = initializeBoard(n, m);

	for (i = 0; i < n * m; i++) {
		for (j = 0; j < n * m; j++) {
			getCell(sudokuCopy, i, j)->value = getCell(sudoku, i, j)->value;
			getCell(sudokuCopy, i, j)->fixed = getCell(sudoku, i, j)->fixed;
			getCell(sudokuCopy, i, j)->error = getCell(sudoku, i, j)->error;
		}
	}

	return sudokuCopy;
}

void copy(SudokuBoard* sudoku, SudokuBoard* sudokuCopy) {
	int i, j, n = sudoku->n, m = sudoku->m;

	for (i = 0; i < n * m; i++) {
		for (j = 0; j < n * m; j++) {
			getCell(sudokuCopy, i, j)->value = getCell(sudoku, i, j)->value;
		}
	}
}

/**** Print functions ****/

void printSeperator(int length) {
	int i;
	for (i = 0; i < length; i++) {
		printf("-");
	}
	printf("\n");
}

void printBoard(SudokuBoard *sudoku, Status mode, int markErrors) {
	int i, j, n, m, N;

	Cell *c;
	n = sudoku->n;
	m = sudoku->m;
	N = n * m;
	printSeperator(4 * N + m + 1);

	for (i = 0; i < N; i++) {
		printf("|");
		for (j = 0; j < N; j++) {
			printf(" ");
			c = getCell(sudoku, i, j);
			if (c->value == 0) {
				printf("   ");
			} else {
				printf("%2d", c->value);
				if (c->fixed == 1 && mode != EDIT) {
					printf(".");
				} else {
					if (c->error == 1 && (mode == EDIT || markErrors == 1)) {
						printf("*");
					} else {
						printf(" ");
					}
				}
			}
			if ((j + 1) % n == 0) {
				printf("|");
			}
		}
		printf("\n");
		if ((i + 1) % m == 0) {
			printSeperator(4 * n * m + m + 1);
		}

	}
}

void printCellUpdate(CommandType command, int i, int j, int oldVal, int newVal) {
	i++;
	j++; /* Change indices to be 1-based */

	if (command == AUTOFILL) {
		printf("Autofill: cell (%d,%d) set to %d.\n", i, j, newVal);
	}

	if (command == GUESS) {
		printf("Guess: cell (%d,%d) set to %d.\n", i, j, newVal);
	}

	if (command == REDO) {
		printf("Redo: changed cell (%d,%d) from %d to %d.\n", i, j, oldVal,
				newVal);
	}

	if (command == UNDO) {
		printf("Undo: changed cell (%d,%d) from %d to %d.\n", i, j, oldVal,
				newVal);
	}
}

void printFormat(CommandType command) {
	printFormatWithRange(command, 0);
}

void printFormatWithRange(CommandType command, int range) {

	printf("Command structure: ");
	if (command == SOLVE_COMMAND) {
		printf("solve X\n");
		printf("X includes a full or relative path to the file.\n");
	}

	if (command == EDIT_WITH_FILE_NAME || command == EDIT_WITHOUT_FILE_NAME) {
		printf("edit X\n");
		printf(
				"X includes a full or relative path to the file, and is an optional parameter.\n");
	}

	if (command == MARK_ERRORS) {
		printf("mark_errors X\n");
		printf("X is 0 or 1.\n");

	}

	if (command == PRINT_BOARD) {
		printf("print_board\n");
	}

	if (command == SET) {
		printf("set X Y Z\n");
		printf(
				"X and Y are integers between 1 to %d, Z is an integer from 0 to %d.\n",
				range, range);
	}

	if (command == VALIDATE) {
		printf("validate\n");
	}

	if (command == GUESS) {
		printf("guess X\n");
		printf("X is a float between 0 to 1.\n");
	}

	if (command == GENERATE) {
		printf("generate X Y\n");
		printf("X and Y are integers between 0 to %d.\n", range);
	}

	if (command == UNDO) {
		printf("undo\n");
	}

	if (command == REDO) {
		printf("redo\n");
	}

	if (command == SAVE) {
		printf("save X\n");
		printf("X includes a full or relative path to the file.\n");
	}

	if (command == HINT) {
		printf("hint X Y\n");
		printf("X and Y are integers between 1 to %d.\n", range);
	}

	if (command == GUESS_HINT) {
		printf("guess_hint X Y\n");
		printf("X and Y are integers between 1 to %d.\n", range);
	}

	if (command == NUM_SOLUTIONS) {
		printf("num_solutions\n");
	}

	if (command == AUTOFILL) {
		printf("autofill\n");
	}

	if (command == RESET) {
		printf("reset\n");
	}

	if (command == EXIT) {
		printf("exit\n");
	}
}

void printGuessHintScore(int val, float score) {
	printf("Value %d has a score of %.2f.\n", val, score);
}

void printInstructionWithParam(CommandType command, int param) {
	if (command == NUM_SOLUTIONS) {
		printf("The board has %d solutions.\n", param);
	}

	if (command == MARK_ERRORS) {
		printf("Mark errors parameter is set to %d.\n", param);
	}

	if (command == HINT) {
		printf("Hint: set cell to %d.\n", param);
	}
}

void printInstruction(Instruction instType) {

	if (instType == WELCOME) {
		printf("Welcome to the sudoku program!\n");
	}

	if (instType == ENTER_COMMAND) {
		printf("Please enter a command: ");
	}

	if (instType == GAME_SAVED) {
		printf("Game saved successfully.\n");
	}

	if (instType == SOLVEABLE) {
		printf("Board is solveable.\n");
	}

	if (instType == UNSOLVEABLE) {
		printf("Board is unsolveable.\n");
	}

	if (instType == EXITING) {
		printf("Hope you enjoyed the game! Exiting...\n");
	}

	if (instType == WIN) {
		printf("Congratulations! You solved the puzzle.\n");
	}

	if (instType == FAKE_WIN) {
		printf(
				"This puzzle is already solved. Please load in edit mode to edit the puzzle.\n");
	}

}

void printError(Error errorType) {
	printf("Error: ");

	/* Errors releated to legal commands (i.e. valid input) */

	if (errorType == FIXED_CELL) {
		printf("cell is fixed.\n");
	}

	if (errorType == ERRONEOUS_BOARD) {
		printf("board is erroneous.\n");
	}

	if (errorType == NO_MOVE_TO_UNDO) {
		printf("there is no move to undo.\n");

	}
	if (errorType == NO_MOVE_TO_REDO) {
		printf("there is no move to redo.\n");
	}

	if (errorType == FILE_NOT_EXIST) {
		printf("the file does not exist.\n");
	}

	if (errorType == INVALID_BOARD_FORMAT) {
		printf("board format is invalid.\n");
	}

	if (errorType == UNSOLVEABLE_BOARD) {
		printf("board is unsolveable.\n");
	}

	if (errorType == CELL_IS_NOT_EMPTY) {
		printf("cell already contains a value.\n");
	}

	if (errorType == MARK_ERRORS_INVALID_VALUE) {
		printf(
				"invalid value for the command's parameter, should be 0 or 1.\n");
	}

	if (errorType == GUESS_INVALID_VALUE) {
		printf(
				"invalid value for the command's parameter, should be a float between 0 to 1.\n");
	}

	if (errorType == UNABLE_TO_VERIFY) {
		printf("Unable to verify board. Saving failed.\n");
	}

	if (errorType == PUZZLE_GENERATOR_FAILED){
		printf("puzzle generator has failed. Please try again.\n");
	}
	/* Errors releated to invalid input (wrong commands or parameters) */

	if (errorType == TOO_MANY_PARAMS) {
		printf("too many parameters for this command.\n");
	}

	if (errorType == NOT_ENOUGH_PARAMS) {
		printf("not enough parameters for this command.\n");
	}

	if (errorType == AVAILABLE_IN_EDIT_AND_SOLVE) {
		printf("this command is available only in Solve and Edit modes.\n");
	}

	if (errorType == AVAILABLE_IN_EDIT) {
		printf("this command is available only in Edit mode.\n");
	}

	if (errorType == AVAILABLE_IN_SOLVE) {
		printf("this command is available only in Solve mode.\n");
	}

	if (errorType == TOO_LONG) {
		printf("a command should contain no more than 256 characters.\n");
	}
	if (errorType == INVALID_COMMAND) {
		printf("unknown command, please try again.\n");
	}

	if (errorType == WRONG_PATH) {
		printf("invalid path.");
	}

	/* System errors */

	if (errorType == FILE_UNHANDLED) {
		printf("couldn't handle file.\n");
	}

	if (errorType == FGET_FAILED) {
		printf("fgets has failed. Terminating the program...\n");
	}

	if (errorType == MALLOC_FAILED) {
		printf("malloc has failed. Terminating the program...\n");
	}

	if (errorType == GUROBI_FAILED) {
		printf("gurobi has encountered an error.\n");
	}

}

void printInvalidParameter(Error errorType, int start, int end) {

	printf("Error: ");

	if (errorType == INVALID_X) {
		printf(
				"invalid value for the first parameter, should be an integer between %d to %d.\n",
				start, end);
	}

	if (errorType == INVALID_Y) {
		printf(
				"invalid value for the second parameter, should be an integer between %d to %d.\n",
				start, end);
	}

	if (errorType == INVALID_Z) {
		printf(
				"invalid value for the third parameter, should be an integer between %d to %d.\n",
				start, end);
	}

	if (errorType == NOT_ENOUGH_EMPTY_CELLS) {
		printf("the board contains only %d empty cells.\n", start);
	}

}

int validateSolution(SudokuBoard* sudoku, Status mode) {
	if (mode == EDIT) {
		return 0;
	}

	if (isSolved(sudoku)) {
		printInstruction(WIN);
		return 1;
	}

	if (isFull(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return 0;
	}

	return 0;
}

int isSolved(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->value == 0) {
				return 0;
			}
		}
	}

	if (isErroneous(sudoku)) {
		return 0;
	}

	return 1;
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Sets cell fields to zero, and value to 'data'.
 * Private function.
 */
void initCell(Cell *c, int data) {
	c->error = 0;
	c->fixed = 0;
	c->value = data;
}

/*
 * Returns 1 iff the board is full (i.e. all cells are filled with a value from 1 to N).
 */
int isFull(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->value == 0) {
				return 0;
			}
		}
	}

	return 1;
}

