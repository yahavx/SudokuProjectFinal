/*
 * Parser.c
 *
 *  Created on: Feb 25, 2019
 *      Author: Orin
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

Command* createIllegalCommand() {
	Command* toReturn = (Command*) malloc(sizeof(Command));
	assertMalloc((void*) toReturn);
	toReturn->command = ILLEGALY_HANDLED_COMMAND;
	return toReturn;
}

/* Creates a new command struct object, and assign given arguments to it.
 * Return a pointer to the new struct.
 * @pre - path is an initialized array, or NULL.
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

Command* parseInput(SudokuBoard* sudoku, Status mode) {
	char str[257];
	char* stream;
	char c[260];
	CommandType cmd;
	int row_size, n, m;
	char command_name[256];
	Command* cmdToReturn;
	char path[256];
	double threshold;
	int params[3] = { 0 };

	if (sudoku != NULL) {
		n = sudoku->n;
		m = sudoku->m;
	} else {
		n = 1;
		m = 1;
	}

	row_size = n * m;

	/*initialization of arrays*/
	initializeArray(c, 259);
	initializeArray(str, 255);
	initializeArray(path, 256);

	if ((c[0] = fgetc(stdin)) == EOF) { /*we reached EOF therefore we will finish according to the forum */
		assertFget();
		cmdToReturn = createCommand(params, path, EXIT, 0);
		return cmdToReturn;
	}

	if (c[0] == '\n') {
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn; /* empty command, skip to the next one, print nothing */
	}
	c[1] = fgetc(stdin);
	assertFget();
	if (c[0] == '\r' && c[1] == '\n') {
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn; /* empty command, skip to the next one, print nothing */
	}

	fgets(str, 256, stdin);
	assertFget();

	strcat(c, str);

	if (strlen(c) > MAX_COMMAND_LENGTH) { /* command contains more than 256 characters */

		finishTheLine(); /* read until the end of the command - not working!!!!!!!!!!*/
		printError(TOO_LONG);
		cmdToReturn = createCommand(params, path, COMMAND_TOO_LONG, 0);
		return cmdToReturn;
	}
	/*from here - all working*/
	stream = strtok(c, " \t\r\n");
	if (stream == NULL) { /* empty command, skip to the next one, print nothing */
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn;
	}
	/* solve command- working */
	if (strcmp(stream, "solve") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkSolveCommand(stream, path)) { /*working*/
			cmdToReturn = createCommand(params, path, SOLVE_COMMAND, 0);
		} else {
			cmdToReturn = createIllegalCommand();

		}
		return cmdToReturn;
	}
	/*edit- working*/
	if (strcmp(stream, "edit") == 0) {/*to fix, not working !*/
		stream = strtok(NULL, " \t\r\n");
		cmd = checkEditCommand(stream, path);
		cmdToReturn = createCommand(params, path, cmd, 0);
		return cmdToReturn;
	}

	/*mark errors- working*/
	if (strcmp(stream, "mark_errors") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkMarkErrorsCommand(stream, mode, params) == 1) {
			cmdToReturn = createCommand(params, path, MARK_ERRORS, 0);

		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*print board- working*/
	if (strcmp(stream, "print_board") == 0) {
		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, PRINT_BOARD, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*set- working  */
	if (strcmp(stream, "set") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkSetCommand(stream, row_size, mode, params) == 1) {
			cmdToReturn = createCommand(params, path, SET, 0);

		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*validate- working*/
	if (strcmp(stream, "validate") == 0) {

		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, VALIDATE, 0);

		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*guess- working*/
	if (strcmp(stream, "guess") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkGuessCommand(stream, mode, &threshold) == 1) {
			cmdToReturn = createCommand(params, path, GUESS, threshold);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*generate- working*/
	if (strcmp(stream, "generate") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkGenerateCommand(stream, row_size, mode, params)) {
			cmdToReturn = createCommand(params, path, GENERATE, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*undo-working*/
	if (strcmp(stream, "undo") == 0) {

		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, UNDO, 0);

		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*redo-working*/
	if (strcmp(stream, "redo") == 0) {

		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, REDO, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*save- working, but printing invalid board error after saving*/
	if (strcmp(stream, "save") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkSaveCommand(stream, mode, path)) {
			cmdToReturn = createCommand(params, path, SAVE, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/* hint- working*/
	if (strcmp(stream, "hint") == 0) {
		safeCopy(stream, command_name);
		stream = strtok(NULL, " \t\r\n");
		if (checkHint_GuessHint_Commands(stream, row_size, mode, params)) {
			cmdToReturn = createCommand(params, path, HINT, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*  guess hint- working, same sigfoult at beggining**/
	if (strcmp(stream, "guess_hint") == 0) {
		safeCopy(stream, command_name);
		stream = strtok(NULL, " \t\r\n");
		if (checkHint_GuessHint_Commands(stream, row_size, mode, params)) {
			cmdToReturn = createCommand(params, path, GUESS_HINT, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/* num solutions-working*/
	if (strcmp(stream, "num_solutions") == 0) {

		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, NUM_SOLUTIONS, 0);

		} else {
			cmdToReturn = createIllegalCommand();

		}
		return cmdToReturn;
	}

	/*autofill- working*/
	if (strcmp(stream, "autofill") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkAutofillCommand(stream, mode)) {
			cmdToReturn = createCommand(params, path, AUTOFILL, 0);
		} else {

			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*reset-working*/
	if (strcmp(stream, "reset") == 0) {

		if (checkSeveralCommands(stream, mode)) {
			cmdToReturn = createCommand(params, path, RESET, 0);
		} else {
			cmdToReturn = createIllegalCommand();
		}
		return cmdToReturn;
	}

	/*exit - working*/
	if (strcmp(stream, "exit") == 0) {
		stream = strtok(NULL, " \t\r\n");
		if (checkParamsNumber(stream, 0) == 0) {
			printFormat(EXIT);
			cmdToReturn = createIllegalCommand();
			return cmdToReturn;
		} else {

			cmdToReturn = createCommand(params, path, EXIT, 0);
			return cmdToReturn;
		}
	}

	printError(INVALID_COMMAND);
	cmdToReturn = createCommand(params, path, UNKNOWN_COMMAND, 0);
	return cmdToReturn;
}

