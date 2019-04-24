/*
 * Parser.c:
 * This module implements Parser.h.
 *
 * Most of the commands verifications are done via ParserAux.
 */

#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "Parser.h"
#include "ParserAux.h"
#include "MainAux.h"
#include "Game.h"
#include "LinkedMovesList.h"

#define  MAX_COMMAND_LENGTH  256
#define  MAX_PARAMS_NUMBER  3

Command* createIllegalCommand();

Command* createCommand(int params[3], char path[256], CommandType cmd,
		double threshold);

/* =============== PUBLIC FUNCTIONS =============== */

void assertFget() {
	if (ferror(stdin)) {
		printError(FGET_FAILED);
		exit(0);
	}
}

int assertFopen(FILE *fp) {
	if (fp == NULL) {
		return 0;
	}
	return 1;
}

Command* parseInput(SudokuBoard* sudoku, Status mode) {
	char str[MAX_COMMAND_LENGTH+1];
	char *stream, *command;
	char c[MAX_COMMAND_LENGTH+4];
	CommandType cmd;
	int row_size, n, m, match;
	char path[MAX_COMMAND_LENGTH];
	double threshold;
	int params[MAX_PARAMS_NUMBER] = { 0 };
	match = 0;
	if (sudoku != NULL) {
		n = sudoku->n;
		m = sudoku->m;
	} else { /*defoult parameters, the board not initialize only in init mode, and their values doesn't matter*/
		n = 1;
		m = 1;
	}

	row_size = n * m;

	/*initialization of arrays*/
	initializeArray(c, MAX_COMMAND_LENGTH+4);
	initializeArray(str,MAX_COMMAND_LENGTH+1);
	initializeArray(path, MAX_COMMAND_LENGTH);

	if ((c[0] = fgetc(stdin)) == EOF) { /*we reached EOF therefore we will finish according to the forum */
		assertFget();
		return createCommand(params, path, EXIT, 0);
	}

	if (c[0] == '\n') {
		return createCommand(params, path, EMPTY_COMMAND, 0); /* empty command, skip to the next one, print nothing */
	}

	c[1] = fgetc(stdin);
	assertFget();
	if (c[0] == '\r' && c[1] == '\n') {
		return createCommand(params, path, EMPTY_COMMAND, 0); /* empty command, skip to the next one, print nothing */
	}

	if (c[1] == '\n') { /*handaling the case of command which is shorter than 2 chars but not empty command */
		if (c[0] != ' ' && c[0] != '\t' && c[0] != '\n' && c[0] != '\v'
				&& c[0] != '\f' && c[0] != 'r') {
			printError(INVALID_COMMAND);
			return createIllegalCommand();
		}
	}

	fgets(str, MAX_COMMAND_LENGTH, stdin);
	assertFget();

	strcat(c, str);
	if ((strlen(c) > MAX_COMMAND_LENGTH)
			&& (c[MAX_COMMAND_LENGTH] != '\0' && c[MAX_COMMAND_LENGTH] != '\n'&& c[MAX_COMMAND_LENGTH] != EOF)) { /* Command contains more than 256 characters */


		finishTheLine(); /* Read until the end of the command*/
		printError(TOO_LONG);
		return createCommand(params, path, COMMAND_TOO_LONG, 0);
	}

	stream = strtok(c, " \t\r\n");
	command = stream;
	stream = strtok(NULL, " \t\r\n"); /* command holds command name, advance stream to next parameter */

	if (command == NULL) { /* First token is NULL, empty command */
		return createCommand(params, path, EMPTY_COMMAND, 0);

	}

	if (strcmp(command, "solve") == 0) { /* Solve command */
		if (validateSolve(stream, path)) {
			return createCommand(params, path, SOLVE_COMMAND, 0);
		}
		match = 1;
	}

	if (strcmp(command, "edit") == 0) { /* Edit command */
		cmd = validateEdit(stream, path);
		return createCommand(params, path, cmd, 0);
	}

	if (strcmp(command, "mark_errors") == 0) { /* Mark errors command */
		if (validateMarkErrors(stream, mode, params)) {
			return createCommand(params, path, MARK_ERRORS, 0);
		}
		match = 1;
	}

	if (strcmp(command, "print_board") == 0) { /* Print board command */
		if (validateZeroParameters(stream, mode, PRINT_BOARD)) {
			return createCommand(params, path, PRINT_BOARD, 0);
		}
		match = 1;
	}

	if (strcmp(command, "set") == 0) { /* Set command */
		if (validateSet(stream, row_size, mode, params)) {
			return createCommand(params, path, SET, 0);
		}
		match = 1;
	}

	if (strcmp(command, "validate") == 0) { /*Validate command*/
		if (validateZeroParameters(stream, mode, VALIDATE)) {
			return createCommand(params, path, VALIDATE, 0);
		}
		match = 1;
	}

	if (strcmp(command, "guess") == 0) { /* Guess command */
		if (validateGuess(stream, mode, &threshold)) {
			return createCommand(params, path, GUESS, threshold);
		}
		match = 1;
	}

	if (strcmp(command, "generate") == 0) { /* Generate command */
		if (validateGenerate(stream, row_size, mode, params)) {
			return createCommand(params, path, GENERATE, 0);
		}
		match = 1;
	}

	if (strcmp(command, "undo") == 0) { /* Undo command */
		if (validateZeroParameters(stream, mode, UNDO)) {
			return createCommand(params, path, UNDO, 0);
		}
		match = 1;
	}

	if (strcmp(command, "redo") == 0) { /* Redo command */
		if (validateZeroParameters(stream, mode, REDO)) {
			return createCommand(params, path, REDO, 0);
		}
		match = 1;
	}

	if (strcmp(command, "save") == 0) { /* Save command */
		if (validateSave(stream, mode, path)) {
			return createCommand(params, path, SAVE, 0);
		}
		match = 1;
	}

	if (strcmp(command, "hint") == 0) { /* Hint command */
		if (validateHintAndGuessHint(stream, row_size, mode, params, HINT)) {
			return createCommand(params, path, HINT, 0);
		}
		match = 1;
	}

	if (strcmp(command, "guess_hint") == 0) { /* Guess hint command*/
		if (validateHintAndGuessHint(stream, row_size, mode, params,
				GUESS_HINT)) {
			return createCommand(params, path, GUESS_HINT, 0);
		}
		match = 1;
	}

	if (strcmp(command, "num_solutions") == 0) { /* Num solutions command */
		if (validateZeroParameters(stream, mode, NUM_SOLUTIONS)) {
			return createCommand(params, path, NUM_SOLUTIONS, 0);

		}
		match = 1;
	}

	if (strcmp(command, "autofill") == 0) { /* Autofill command */
		if (validateAutofill(stream, mode)) {
			return createCommand(params, path, AUTOFILL, 0);
		}
		match = 1;
	}

	if (strcmp(command, "reset") == 0) { /* Reset command */
		if (validateZeroParameters(stream, mode, RESET)) {
			return createCommand(params, path, RESET, 0);
		}
		match = 1;
	}

	if (strcmp(command, "exit") == 0) { /* Exit command */
		if (validateParamsNumber(stream, 0)) {
			return createCommand(params, path, EXIT, 0);
		}
		printFormat(EXIT);
		match = 1;
	}

	if (!match) { /* Command name didn't match any command */
		printError(INVALID_COMMAND);
	}

	return createIllegalCommand();
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Creates a new command struct object which represent ILLEGALY_HANDALED_COMMAND.
 * Return a pointer to it.
 */
Command* createIllegalCommand() {
	Command* toReturn = (Command*) malloc(sizeof(Command));
	assertMalloc((void*) toReturn);
	toReturn->command = ILLEGALY_HANDLED_COMMAND;
	return toReturn;
}

/* Creates a new command struct object, and assign given arguments to it.
 * Return a pointer to the new struct.
 * @pre - path is an initialized array.
 */
Command* createCommand(int params[3], char path[256], CommandType cmd,
		double threshold) {
	Command* toReturn = (Command*) malloc(sizeof(Command));
	assertMalloc((void*) toReturn);
	toReturn->command = cmd;
	toReturn->params[0] = params[0];
	toReturn->params[1] = params[1];
	toReturn->params[2] = params[2];
	strcpy(toReturn->path, path);
	toReturn->threshold = threshold;
	return toReturn;
}
