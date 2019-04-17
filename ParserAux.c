/*
 * ParserAux.c
 *
 *  Created on: Feb 23, 2019
 *      Author: Orin
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

int isValidNumber(char* stream, int n) {
	int x;
	int i = 0;
	while (stream[i] != '\0') {
		if (!(stream[i] >= '0' && stream[i] <= '9')) {
			return 0;
		}
		i++;
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

int isNumInRange(int num, int beginning, int end) {
	if (num >= beginning && num <= end) {
		return 1;
	}
	return 0;
}

int numOfArguments(char* stream) {
	int cnt = 0;

	/* should not be null at any case because we already  */

	while (stream != NULL) {
		cnt++;

		stream = strtok(NULL, " \t\r\n");/*moving stream one token forward*/

	}

	return cnt;
}

void safeCopy(char* stream, char* path) {
	int i = 0;
	if (stream == NULL || path == NULL) {
		return;
	}
	while (stream[i] != '\0') {
		path[i] = stream[i];
		i++;
	}
	path[i] = '\0';
}

int checkSolveCommand(char* stream, char* path) {

	int cnt;
	if (stream == NULL) {
		printError(NOT_ENOUGH_PARAMS);
		printFormat(SOLVE_COMMAND);
		return 0;
	}
	safeCopy(stream, path);
	cnt = numOfArguments(stream);
	if (cnt == 1) {
		return 1;
	} else {
		if (cnt == 0) {
			printError(NOT_ENOUGH_PARAMS);
		} else {
			printError(TOO_MANY_PARAMS);
		}
		printFormat(SOLVE_COMMAND);
		return 0;
	}
}

CommandType checkEditCommand(char* stream, char* path) {
	int cnt;

	if (stream == NULL) {
		return EDIT_WITHOUT_FILE_NAME;
	}
	safeCopy(stream, path);
	cnt = numOfArguments(stream);
	if (cnt == 1) {
		return EDIT_WITH_FILE_NAME;

	} else {
		printError(TOO_MANY_PARAMS);
		printFormat(EDIT_WITH_FILE_NAME);
		return ILLEGALY_HANDLED_COMMAND;
	}

}

int checkMarkErrorsCommand(char* stream, Status mode, int params[3]) {

	int cnt, wrong_value_for_x;
	wrong_value_for_x = 0;

	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	if (stream == NULL) {
		printError(NOT_ENOUGH_PARAMS);
		printFormat(MARK_ERRORS);
		return 0;
	}

	if (!(strcmp(stream, "0") == 0 || strcmp(stream, "1") == 0)) {
		wrong_value_for_x = 1;
	} else {
		params[0] = atoi(stream);
	}

	cnt = numOfArguments(stream);
	if (cnt > 1) {
		printError(TOO_MANY_PARAMS);
		printFormat(MARK_ERRORS);
		return 0;
	}
	if (wrong_value_for_x) {
		printError(MARK_ERRORS_INVALID_VALUE);
		return 0;
	}
	return 1;

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

int checkParamsNumber(char* stream, int paramsNum) {
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

int checkSeveralCommands(char* stream, Status mode) {
	char command[256];
	safeCopy(stream, command);
	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}
	stream = strtok(NULL, " \t\r\n");
	if (checkParamsNumber(stream, 0) == 0) {/*print error already handaled*/
		printf("Command structure : %s\n", command);
		return 0;
	}
	return 1;
}

int checkSetCommand(char* stream, int range, Status mode, int params[3]) {
	int num;
	int count_params = 0;

	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}

	while (stream != NULL && count_params < 2) {
		num = getNum(stream);
		params[count_params] = num - 1;
		count_params++;
		stream = strtok(NULL, " \t\r\n");
	}

	if (count_params < 2) {
		printError(NOT_ENOUGH_PARAMS);
		printFormatWithRange(SET, range);
		return 0;
	}

	if (stream != NULL) {
		num = getNum(stream);
		params[2] = num;
	} else {
		printError(NOT_ENOUGH_PARAMS);
		printFormatWithRange(SET, range);
		return 0;
	}
	stream = strtok(NULL, " \t\r\n");
	if (checkParamsNumber(stream, 0) == 0) {/*print already*/
		printFormatWithRange(SET, range);
		return 0;
	}

	if (!(isNumInRange(params[0] + 1, 1, range))) {
		printErrorWithRange(INVALID_X, 1, range);
		return 0;
	}

	if (!(isNumInRange(params[1] + 1, 1, range))) {
		printErrorWithRange(INVALID_Y,1,range);
		return 0;
	}
	if (!(isNumInRange(params[2], 0, range))) {
		printErrorWithRange(INVALID_Z,0,range);
		return 0;
	}

	return 1;

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
				return 0;/*or there is more than one point or the char is illegal. */
			}
		}

		i++;
	}
	return 1;
}

int checkGuessCommand(char* stream, Status mode, double* threshold) {
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

	if (!isFloat(stream)) {
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
	if (!checkParamsNumber(stream, 0)) {/*prints*/
		printFormat(GUESS);
		return 0;
	}
	if (wrong_value_for_X) {
		printError(GUESS_INVALID_VALUE);
		return 0;
	}

	return 1;
}

int checkGenerateCommand(char* stream, int range, Status mode, int params[3]) {
	int num, count_params;
	count_params = 0;

	if (mode != EDIT) {
		printError(AVAILABLE_IN_EDIT);
		return 0;
	}
	while (stream != NULL && count_params < 2) {
		num = getNum(stream);
		params[count_params] = num;
		count_params++;
		stream = strtok(NULL, " \t\r\n");
	}


	if (count_params < 2) {
		printError(NOT_ENOUGH_PARAMS);
		printFormatWithRange(GENERATE,range * range);
		return 0;
	}

	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		printFormatWithRange(GENERATE, range * range);
		return 0;
	}
	if (!(isNumInRange(params[0], 0, range * range))) {
		printErrorWithRange(INVALID_X,0,range*range);
		return 0;
	}
	if (!(isNumInRange(params[1], 0, range * range))) {
		printErrorWithRange(INVALID_Y,0, range*range);
		return 0;
	}

	return 1;
}

int checkHint_GuessHint_Commands(char* stream, int range, Status mode,
		int params[3]) {/*wowrking*/
	int num, count_params = 0;

	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	while (stream != NULL && count_params < 2) {
		num = getNum(stream);
		params[count_params] = num - 1;
		count_params++;
		stream = strtok(NULL, " \t\r\n");
	}

	if (count_params < 2) {
		printError(NOT_ENOUGH_PARAMS);
		printFormatWithRange(GUESS_HINT,range);
		return 0;
	}

	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		printFormatWithRange(GUESS_HINT,range);
		return 0;
	}

	if (!(isNumInRange(params[0] + 1, 1, range))) {
		printErrorWithRange(INVALID_X,1,range);
		return 0;
	}
	if (!(isNumInRange(params[1] + 1, 1, range))) {
		printErrorWithRange(INVALID_Y,1,range);
		return 0;
	}

	return 1;
}

int checkAutofillCommand(char* stream, Status mode) { /*working*/
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}
	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		printFormat(AUTOFILL);
		return 0;
	}
	return 1;
}

int checkSaveCommand(char* stream, Status mode, char path[256]) {/*working*/
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
	if (checkParamsNumber(stream, 1) == 0) {/*prints*/
		printFormat(SAVE);
		return 0;
	}
	return 1;
}

