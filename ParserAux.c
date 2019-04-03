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

int isValidNumber(char* stream, int n) { /* already checked, works */
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
	if (isNum(string)) {
		x = atoi(string);
		return x;
	}

	return -1;
}

int isNumInRange(int num, int beggining, int end) {
	if (num >= beggining && num <= end) {
		return 1;
	}
	return 0;
}

int numOfArguments(char* stream) {
	int cnt = 0;

	while (stream != NULL) {
		cnt++;
		stream = strtok(NULL, " \t\r\n");/*moving stream one token forward*/
	}

	return cnt;
}

int checkSolveCommand(char* stream) {
	int cnt = numOfArguments(stream);

	if (cnt == 1) {
		return 1;
	}

	else {
		if (cnt == 0) {
			printError(TOO_MANY_PARAMS);
		}

		else {
			printError(NOT_ENOUGH_PARAMS);
		}
		printf("Command format: solve <filepath>./n");
		return 0;
	}
}

CommandType checkEditCommand(int paramsNum) {
	if (paramsNum == 0) {
		return EDIT_WITHOUT_FILE_NAME;
	} else {
		if (paramsNum == 1) {
			return EDIT_WITH_FILE_NAME;
		} else {
			printError(TOO_MANY_PARAMS);
			printf("Command format: edit <filepath>.\n");
			return ILLEGALY_HANDLED_COMMAND;
		}
	}
}

int checkMarkErrorsCommand(char* stream, Status mode) {

	int cnt = numOfArguments(stream);
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}
	if (cnt == 0) {
		printError(NOT_ENOUGH_PARAMS);
		printf(
				"command structure : mark_errors X , when X can be 1 or 0 only /n");
		return 0;
	}
	if (cnt > 1) {
		printError(TOO_MANY_PARAMS);
		printf(
				"command structure : mark_errors X , when X can be 1 or 0 only /n");
		return 0;
	}

	if (stream != NULL) {/*should not be NULL at any case*/
		if (!(strcmp(stream, "0") == 0 || strcmp(stream, "1") == 0)) {
			printf(
					"mark_errors X command got wrong value for the argument X /n");
			printf("X should be 1 or 0 only /n");
			return 0;
		} else {
			return 1;
		}
	} else {
		return 0; /*should never get there*/
	}
}

void finishTheLine() {
	char c[258];

	while (c[257] != '\0') {
		initializeArray(c, 258);
		fgets(c, 258, stdin);
	}
}

/*
 * Returns the number of words in the stream, counts up to N*N.
 * @pre - 'longest' is the longest word in the stream.
 */
int wordsCount(FILE *input, int longest, int N) {

	char *str = malloc((longest + 1) * sizeof(char));
	int count = 0;

	assertMalloc((void*) str);

	while (fscanf(input, "%s", str) != EOF) {
		count++;
		if (count == N * N + 2) { /* need to verify the first NxN words (excluding n and m) */
			break;
		}
	}
	free(str);
	return count;
}

/*
 * Checks if word represents a valid cell format, i.e. number is in range 0-N, possibly with dot at the end.
 */
int isValidCellFormat(char* word, int N) {
	int x;
	int i = 0;
	int fixed = 0;

	while (word[i] != '\0') {
		if (!(word[i] >= '0' && word[i] <= '9')) { /* not a digit */
			if (word[i] == '.' && word[i + 1] == '\0') {
				fixed = 1;
				break; /* a number followed by a dot, legal cell (fixed) */
			}
			/*printf("illegal word\n");*/
			return 0;
		}
		i++;
	}

	x = atoi(word); /* will ignore the dot, if exists */

	if (x == 0 && fixed == 1) { /* empty cell cannot be fixed, and a dot without a number before is also illegal */
		return 0;
	}

	if (x >= 0 && x <= N) {
		return 1;
	}
	/*printf("number not in range\n");*/
	return 0; /* number is not in range */
}

/*
 * Returns number of digits in num.
 */
int digitsInNum(int num) {
	int count = 0;
	while (num != 0) {
		count++;
		num /= 10;
	}
	return count;
}
/*
 * Returns the length of the longest word in the file, starting to read from where the pointer is.
 * @pre - file is open.
 */
int longestWord(FILE *input) {
	int c, length = 0;
	int longest = 0;
	while ((c = fgetc(input)) != -1) {
		if (!isspace(c)) {
			length++;
		} else {
			if (longest < length) {
				longest = length;
			}
			length = 0;
		}
	}

	if (longest < length) /* last word is the longest */
		return length;
	return longest;
}

int isValidBoardFormat(FILE* input) {
	int debug = 1;
	int n, m, N, wordsNum, count, longest;
	char *stream;

	longest = longestWord(input);

	if (longest == 0) {
		if (debug) {
			printf("file is empty\n");
		}
		return 0; /* file contain only whitespaces */
	}

	rewind(input);
	if (debug)
		printf("longest word in the file: %d\n", longest);
	stream = malloc((longest + 1) * sizeof(char));
	assertMalloc((void*) stream);

	if (fscanf(input, "%s", stream) == -1 || !isValidNumber(stream, 5)) { /* reached EOF while reading n */
		free(stream);
		if (debug)
			printf("n is erroneous\n");
		return 0;

	}

	n = atoi(stream);

	if (fscanf(input, "%s", stream) == -1 || !isValidNumber(stream, 5)) { /* reached EOF while reading n */
		free(stream);
		if (debug)
			printf("m is erroneous\n");
		return 0;

	}

	m = atoi(stream);

	if (n == 0 || m == 0) { /* isValidNumber allowes zero */
		free(stream);
		if (debug)
			printf("n or m are zero\n");
		return 0;
	}

	N = n * m;

	rewind(input);

	wordsNum = wordsCount(input, longest, N);/* retrieves the number of words in the file */
	wordsNum -= 2; /* exclude n and m */

	if (wordsNum == 0 || wordsNum < N * N) { /* found an illegal word, or not enough information */
		free(stream);
		if (debug)
			printf("not enough cells: %d\n", wordsNum);
		return 0;
	}

	count = 0; /* we need to verify the first NxN words (ignore the rest) */

	rewind(input);
	fscanf(input, "%s", stream);
	fscanf(input, "%s", stream); /* skip n and m */

	while (fscanf(input, "%s", stream) != EOF) {
		if (!isValidCellFormat(stream, N)) {
			if (debug)
				printf("invalid cells format\n");
			free(stream);
			return 0;
		}
		count++;
		if (count == N * N) {
			break;
		}
	}

	free(stream);
	return 1;
}

int exceptedNumOfParams(int paramsNum, int excepted) {
	if (excepted < paramsNum) {
		printError(TOO_MANY_PARAMS);
		return 0;
	}

	if (excepted > paramsNum) {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}
	return 1;
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
	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}

	if (checkParamsNumber(stream, 0) == 0) {/*print error already handaled*/
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

		if ((num != -1) && (isNumInRange(num, 1, range))) {
			params[count_params] = num - 1;
		} else {
			/*need to add print error of wrong param value !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1*/
			return 0;
		}

		count_params++;
		stream = strtok(NULL, " \t\r\n");

	}

	if (count_params != 1) {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}

	if (stream != NULL) {
		num = getNum(stream);
		if ((num != -1) && (isNumInRange(num, 0, range))) {
			params[2] = num;
		} else {
			/*need to add print error of wrong param value !!!!!!!!!!!!!!!!!!!!!!!!!!!11*/
			return 0;
		}
	} else {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}

	stream = strtok(NULL, " \t\r\n");
	if (checkParamsNumber(stream, 0) == 0) {/*print already*/
		return 0;
	}

	return 1;

}

int isFlout(char * stream) {
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

int cheackGuessCommand(char* stream, Status mode) {
	int x;
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}
	if (stream != NULL) {
		if (!isFlout(stream)) {
			printf(
					"guess command should get fouat between 0 to 1 as parameter");
			return 0;
		} else {
			x = atof(stream);
			if (!(x >= 0 && x <= 1)) {
				printf(
						"guess command should get fouat between 0 to 1 as parameter");
				return 0;
			}
		}

		stream = strtok(NULL, " \t\r\n");
		if (!checkParamsNumber(stream, 0)) {/*prints*/
			return 0;
		}
	} else {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}

	return 1;
}

int checkGenerateCommand(char* stream, int range, Status mode, int params[3]) {
	int num;
	int count_params = 0;

	if (mode != EDIT) {
		printError(AVAILABLE_IN_EDIT);
		return 0;
	}

	while (stream != NULL && count_params < 2) {
		num = getNum(stream);
		if ((num != -1) && (isNumInRange(num, 0, range * range))) {
			params[count_params] = num;
		} else {
			/*need to add print error of wrong param value !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1*/
			return 0;
		}
		count_params++;
		stream = strtok(NULL, " \t\r\n");
	}

	if (count_params != 1) {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}

	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		return 0;
	}
	return 1;
}

int checkHint_GuessHint_Commands(char* stream, int range, Status mode,
		int params[3]) {
	int num;
	int count_params = 0;

	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}

	while (stream != NULL && count_params < 2) {
		num = getNum(stream);
		if ((num != -1) && (isNumInRange(num, 1, range))) {
			params[count_params] = num - 1;
		} else {
			/*need to add print error of wrong param value !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1*/
			return 0;
		}
		count_params++;
		stream = strtok(NULL, " \t\r\n");
	}

	if (count_params != 1) {
		printError(NOT_ENOUGH_PARAMS);
		return 0;
	}

	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		return 0;
	}
	return 1;
}

int checkAutofillCommand(char* stream, Status mode) {
	if (mode != SOLVE) {
		printError(AVAILABLE_IN_SOLVE);
		return 0;
	}
	if (checkParamsNumber(stream, 0) == 0) {/*prints*/
		return 0;
	}
	return 1;
}

int checkSaveCommand(char* stream, Status mode) {
	int d;
	if (mode == INIT) {
		printError(AVAILABLE_IN_EDIT_AND_SOLVE);
		return 0;
	}
	if ((d = checkParamsNumber(stream, 1)) == 0) {/*prints*/
		printf("%d parmas, return 0\n", d);
		return 0;
	}

	printf("%d parmas, return 1\n", d);
	return 1;
}

