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

void assertFget() {
	if (ferror(stdin)) {
		printError(FGET_FAILED);
		exit(0);
	}
}

int assertFopen(FILE *fp) {
	if (fp == NULL) {
		printError(FILE_UNHANDLED);
		return 0;
	}
	return 1;
}

Command* createCommand(int params[3], char* path, CommandType cmd,
		double threshold) {
	Command* toReturn = (Command*) malloc(sizeof(Command));
	toReturn->command = cmd;

	toReturn->params[0] = params[0];
	toReturn->params[1] = params[1];
	toReturn->params[2] = params[2];
	toReturn->path = path;
	toReturn->threshold = threshold;
	return toReturn;
}

Command* parseInput(SudokuBoard* sudoku, Status mode) {
	char str[257];
	char* stream, *command;
	char* streamCopy;
	char c[259], c2[259];
	CommandType cmd;
	int params[3] = { 0 }, args = 0, error = 0;

	Command* cmdToReturn;
	char* path = NULL;

	initializeArray(c, 259);

	if ((c[0] = fgetc(stdin)) == EOF) { /*we reached EOF therefore we will finish according to the forum */
		assertFget();
		cmdToReturn = createCommand(params, path, EXIT, 0);
		return cmdToReturn;
	}

	if (c[0] == '\n') {
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn; /* empty command, skip to the next one, print nothing */
	}

	if (c[0] == '\r' && (c[1] = fgetc(stdin) == '\n')) {
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn; /* empty command, skip to the next one, print nothing */
	} else {
		assertFget();
	}

	fgets(str, 257, stdin);
	assertFget();

	/* checking number of characters in the command */
	strcat(c, str);

	if (c[258] != '\0' && c[258] != '\n') { /* command contains more than 256 characters */
		finishTheLine(); /* read until the end of the command */
		cmdToReturn = createCommand(params, path, COMMAND_TOO_LONG, 0);
		return cmdToReturn;
	}

	/* finished reading, and input is in a legal length */

	strcpy(c2, c);
	if (0)
		printf("line: %s\n", c);
	stream = strtok(c2, " \t\r\n");
	args = numOfArguments(stream) - 1; /* excluding first token (command name) */
	if (0)
		printf("args = %d\n", args);

	stream = strtok(c, " \t\r\n");
	command = stream;
	stream = strtok(NULL, " \t\r\n");
	if (0)
		printf("\ncommand: %s, current token: %s\n", command, stream);

	if (stream == NULL) { /* empty command, skip to the next one, print nothing */
		cmdToReturn = createCommand(params, path, EMPTY_COMMAND, 0);
		return cmdToReturn;
	}

	if (strcmp(command, "solve") == 0) { /* solve command */

		if (exceptedNumOfParams(args, 1)) {
			strcpy(command, stream);
			cmdToReturn = createCommand(params, command, SOLVE_COMMAND, 0); /* current token in stream is the first parameter */
			return cmdToReturn;
		} else {
			error = 1;
		}

	}

	if (strcmp(stream, "edit") == 0) { /* edit command */
		cmd = checkEditCommand(args);
		if (cmd == EDIT_WITH_FILE_NAME) {
			cmdToReturn = createCommand(params, stream, EDIT_WITH_FILE_NAME, 0);
		} else {
			cmdToReturn = createCommand(params, path, EDIT_WITHOUT_FILE_NAME,
					0);
			return cmdToReturn;
		}
	}
	streamCopy = strtok(c, "");
	/*mark errors*/
	if (strcmp(stream, "mark_errors") == 0) {
		if (checkMarkErrorsCommand(streamCopy, mode)) {
			stream = strtok(NULL, " \t\r\n");
			if (stream != NULL) {/*should never be NULL at this point*/
				params[0] = atoi(stream);
				cmdToReturn = createCommand(params, path, MARK_ERRORS, 0);
				return cmdToReturn;
			} else {
				cmdToReturn = createCommand(params, path,
						ILLEGALY_HANDLED_COMMAND, 0);
				return cmdToReturn;
			}
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
			return cmdToReturn;
		}
	}

	/*print board*/
	if (strcmp(stream, "print_board") == 0) {
		cmdToReturn = createCommand(params, path, PRINT_BOARD, 0);
					return cmdToReturn;
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, PRINT_BOARD, 0);
			return cmdToReturn;
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
			return cmdToReturn;
		}
	}

	/*set */
	if (strcmp(stream, "set") == 0) {
		if (checkSetCommand(streamCopy, mode, sudoku->n, params) == 1) {
			cmdToReturn = createCommand(params, path, SET, 0);

		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*validate*/
	if (strcmp(stream, "validate") == 0) {
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, VALIDATE, 0);

		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*guess*/
	if (strcmp(stream, "guess") == 0) {
		if (cheackGuessCommand(streamCopy, mode)) {
			stream = strtok(NULL, " \t\r\n");
			if (stream != NULL) { /*shoultd never be*/
				cmdToReturn = createCommand(params, path, GUESS, atof(stream));
			}
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}

		return cmdToReturn;
	}

	/*generate*/
	if (strcmp(stream, "generate") == 0) {
		if (checkGenerateCommand(streamCopy, sudoku->n, mode, params)) {
			cmdToReturn = createCommand(params, path, GENERATE, 0);
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*undo*/
	if (strcmp(stream, "undo") == 0) {
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, UNDO, 0);
			return cmdToReturn;
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
			return cmdToReturn;
		}
	}

	/*redo*/
	if (strcmp(stream, "redo") == 0) {
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, REDO, 0);
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*save*/
	if (strcmp(stream, "save") == 0) {
		if (checkSaveCommand(streamCopy, mode)) {
			stream = strtok(NULL, " \t\r\n");
			cmdToReturn = createCommand(params, stream, SAVE, 0);
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/* hint*/
	if (strcmp(stream, "hint") == 0) {
		if (checkHint_GuessHint_Commands(streamCopy, sudoku->n, mode, params)) {
			cmdToReturn = createCommand(params, path, HINT, 0);
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*  guess hint*/
	if (strcmp(stream, "guess_hint") == 0) {
		if (checkHint_GuessHint_Commands(streamCopy, sudoku->n, mode, params)) {
			cmdToReturn = createCommand(params, path, GUESS_HINT, 0);
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/* num solutions*/
	if (strcmp(stream, "num_solutions") == 0) {
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, NUM_SOLUTIONS, 0);

		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);

		}
		return cmdToReturn;
	}

	/*autofill*/
	if (strcmp(stream, "autofill") == 0) {
		if (checkAutofillCommand(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, AUTOFILL, 0);
		} else {

			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
		}
		return cmdToReturn;
	}

	/*reset*/
	if (strcmp(stream, "reset") == 0) {
		if (checkSeveralCommands(streamCopy, mode)) {
			cmdToReturn = createCommand(params, path, RESET, 0);
			return cmdToReturn;
		} else {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
			return cmdToReturn;
		}
	}

	/*exit*/
	if (strcmp(stream, "exit") == 0) {
		if (checkParamsNumber(streamCopy, 0) == 0) {
			cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND,
					0);
			return cmdToReturn;
		} else {
			cmdToReturn = createCommand(params, path, EXIT, 0);
			return cmdToReturn;
		}
	}

	if (error)
		cmdToReturn = createCommand(params, path, ILLEGALY_HANDLED_COMMAND, 0);
	return cmdToReturn; /* in case of unknown command, need to print error message from main */
}

