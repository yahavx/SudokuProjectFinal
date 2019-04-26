/*
 * ParserAux.c:
 * This module implements ParserAux.h.
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "ctype.h"
#include "string.h"
#include "MainAux.h"
#include "Parser.h"
#include "Solver.h"
#include "LinkedMovesList.h"
#include "ParserAux.h"

int getIntParams(char *stream, int params[3]);

int expectedNumOfParams(int excepted, int paramsNum, CommandType command,
		int range);

int argsInStream(char* stream);

/* =============== PUBLIC FUNCTIONS =============== */

void initializeArray(char c[], int N) {
	int i;
	for (i = 0; i < N; i++) {
		c[i] = '\0';
	}
}

int isValidNumber(char* stream, int n) {
	int x;

	if (!isNum(stream)) {
		return 0;
	}

	x = atoi(stream);
	if (x >= 0 && x <= n) {
		return 1;
	}
	return 0;
}

int isNum(char* stream) {
	int i = 0;
	while (stream[i] != '\0') {
		if (!(stream[i] >= '0' && stream[i] <= '9')) {
			return 0;
		}
		i++;
	}
	return 1;
}

int getNum(char* string) {
	int x;
	if (string != NULL) {
		if (isNum(string)) {
			x = atoi(string);
			return x;
		}
	}
	return -1;
}

int isFloat(char* stream) {
	int bol, i;
	i = 0;
	bol = 0;
	while (stream[i] != '\0') {
		if (!(stream[i] >= '0' && stream[i] <= '9')) {
			if (stream[i] == '.' && bol == 0) {
				bol = 1;
			} else {
				return 0; /* More than one point, or an invalid character */
			}
		}

		i++;
	}
	return 1;
}

int isNumInRange(int num, int beginning, int end) {
	if (num >= beginning && num <= end) {
		return 1;
	}
	return 0;
}

void finishTheLine() {
	char c;
	c = fgetc(stdin);
	assertFget();
	while (c != '\0' && c != '\n' && c != EOF) {
		c = fgetc(stdin);
		assertFget();
	}
}

void safeCopy(char* src, char* dst) {
	int i = 0;
	if (src == NULL || dst == NULL) {
		return;
	}
	while (src[i] != '\0') {
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
}

int validateSolve(char* stream, char* path) {

	int paramsNum;
	if (stream == NULL) {
		printError(NOT_ENOUGH_PARAMS);
		printFormat(SOLVE_COMMAND);
		return 0;
	}
	safeCopy(stream, path);
	paramsNum = argsInStream(stream);

	if (!expectedNumOfParams(1, paramsNum, SOLVE_COMMAND, 0)) {
		return 0;
	}

	return 1;
}

CommandType validateEdit(char* stream, char* path) {
	int paramsNum;

	if (stream == NULL) {
		return EDIT_WITHOUT_FILE_NAME;
	}

	safeCopy(stream, path);
	paramsNum = argsInStream(stream);
	if (paramsNum == 1) {
		return EDIT_WITH_FILE_NAME;

	} else {
		printError(TOO_MANY_PARAMS);
		printFormat(EDIT_WITH_FILE_NAME);
		return ILLEGALY_HANDLED_COMMAND;
	}

}

int validateMarkErrors(char* stream, Status mode, int params[3]) {

	int count;

	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	count = getIntParams(stream, params);

	if (!expectedNumOfParams(1, count, MARK_ERRORS, 0)) { /* Incorrect number of parameters */
		return 0;
	}

	if (params[0] != 1 && params[0] != 0) {
		printError(MARK_ERRORS_INVALID_VALUE);

		return 0;
	}

	return 1;
}

int validateParamsNumber(char* stream, int paramsNum) {
	int cnt = 0;
	while (stream != NULL) {
		cnt++;
		stream = strtok(NULL, " \t\r\n");
	}

	if (cnt == paramsNum) {
		return 1;
	}

	if (cnt > paramsNum) {
		printError(TOO_MANY_PARAMS);
		return 0;
	}

	printError(NOT_ENOUGH_PARAMS);
	return 0;
}

int validateZeroParameters(char* stream, Status mode, CommandType command) {
	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}

	if (!validateParamsNumber(stream, 0)) {/* Incorrect number of parameters */
		printFormat(command);
		return 0;
	}
	return 1;
}

int validateSet(char* stream, int range, Status mode, int params[3]) {
	int paramsNum;

	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}

	paramsNum = getIntParams(stream, params);

	if (!expectedNumOfParams(3, paramsNum, SET, range)) {
		return 0;
	}

	params[0]--; /* User enters 1-based indices, change it to be 0-based */
	params[1]--;

	if (!(isNumInRange(params[0] + 1, 1, range))) {
		printInvalidParameter(INVALID_X, 1, range);
		return 0;
	}

	if (!(isNumInRange(params[1] + 1, 1, range))) {
		printInvalidParameter(INVALID_Y, 1, range);
		return 0;
	}
	if (!(isNumInRange(params[2], 0, range))) {
		printInvalidParameter(INVALID_Z, 0, range);
		return 0;
	}

	return 1;
}

int validateGuess(char* stream, Status mode, double* threshold) {
	int wrong_value_for_X = 0;
	double x;
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	if (stream == NULL) {
		printError(NOT_ENOUGH_PARAMS);
		printFormat(GUESS);
		return 0;
	}

	if (!isFloat(stream)) { /* Check if the parameter is a float, and convert to float if yes */
		wrong_value_for_X = 1;
	} else {
		x = atof(stream);
		if (!(x >= 0 && x <= 1)) {
			wrong_value_for_X = 1;
		} else {
			*threshold = x;
		}
	}

	stream = strtok(NULL, " \t\r\n");
	if (!validateParamsNumber(stream, 0)) { /* Too many parameters */
		printFormat(GUESS);
		return 0;
	}
	if (wrong_value_for_X) {
		printError(GUESS_INVALID_VALUE);
		return 0;
	}

	return 1;
}

int validateGenerate(char* stream, int range, Status mode, int params[3]) {
	int paramsNum;

	if (mode != EDIT) {
		printError(AVAILABLE_IN_EDIT);
		return 0;
	}

	paramsNum = getIntParams(stream, params);

	if (!expectedNumOfParams(2, paramsNum, GENERATE, range * range)) {
		return 0;
	}

	if (!(isNumInRange(params[0], 0, range * range))) {
		printInvalidParameter(INVALID_X, 0, range * range);
		return 0;
	}
	if (!(isNumInRange(params[1], 0, range * range))) {
		printInvalidParameter(INVALID_Y, 0, range * range);
		return 0;
	}

	return 1;
}

int validateHintAndGuessHint(char* stream, int range, Status mode,
		int params[3], CommandType command) {
	int paramsNum;

	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	paramsNum = getIntParams(stream, params);

	if (!expectedNumOfParams(2, paramsNum, command, range)) {
		return 0;

	}

	params[0]--; /* User enters 1-based indices, change it to be 0-based */
	params[1]--;

	if (!(isNumInRange(params[0] + 1, 1, range))) {
		printInvalidParameter(INVALID_X, 1, range);
		return 0;
	}
	if (!(isNumInRange(params[1] + 1, 1, range))) {
		printInvalidParameter(INVALID_Y, 1, range);
		return 0;
	}

	return 1;
}

int validateAutofill(char* stream, Status mode) { /*working*/
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}
	if (!validateParamsNumber(stream, 0)) { /* Too many parameters */
		printFormat(AUTOFILL);
		return 0;
	}
	return 1;
}

int validateSave(char* stream, Status mode, char path[256]) {/*working*/
	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}
	if (stream == NULL) {
		printError(NOT_ENOUGH_PARAMS);
		printFormat(SAVE);
		return 0;
	}
	safeCopy(stream, path);
	if (!validateParamsNumber(stream, 1)) {/*prints*/
		printFormat(SAVE);
		return 0;
	}
	return 1;
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Checks if excepted number of parameter matches the actual number of parameters supplied by the user.
 * If yes, returns 1, otherwise returns 0, prints appropriate error and correct command format.
 *
 * @params: excepted - number of parameters that a command needs.
 *          paramsNum - number of parameters received by the user.
 *          command - command of which format will be printed if needed.
 *          range - range for format, if needed (if unnecessary, it's ignored).
 */

int expectedNumOfParams(int excepted, int paramsNum, CommandType command,
		int range) {
	if (paramsNum == excepted) {
		return 1;
	}

	if (paramsNum < excepted) {
		printError(NOT_ENOUGH_PARAMS);
	} else {
		printError(TOO_MANY_PARAMS);
	}

	printFormatWithRange(command, range);
	return 0;

}
/*
 * Receives a stream and store its tokens in params, in order (params[0] = first token).
 * If a token is not a valid integer, puts -1 in its place in params.
 *
 * If stream contains less than 3 tokens, any empty slot in params will be set to -2.
 *
 * Returns the number of tokens in the stream.
 */
int getIntParams(char *stream, int params[3]) {
	int i = 0, count = 0;

	while (stream != NULL && i < 3) {
		params[i] = getNum(stream);
		i++;
		count++;
		stream = strtok(NULL, " \t\r\n");

	}

	while (i < 3) { /* Fill the empty array slots with -1 */
		params[i] = -1;
		i++;
	}

	while (stream != NULL) { /* Count extra parameters if exists */
		count++;
		stream = strtok(NULL, " \t\r\n");
	}

	return count;
}

/*
 * Receives a stream and returns the number of tokens in the stream.
 */
int argsInStream(char* stream) {
	int cnt = 0;

	while (stream != NULL) {
		cnt++;

		stream = strtok(NULL, " \t\r\n"); /* Moving stream one token forward */

	}

	return cnt;
}
