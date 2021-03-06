/*
 * FileHandle.c:
 * This module implements FileHandle.h.
 */

#include <stdlib.h>
#include "ctype.h"
#include "unistd.h"
#include "MainAux.h"
#include "ParserAux.h"
#include "GameAux.h"
#include "Parser.h"

#define MAX_M 5
#define MAX_N 5

int isValidBoardFormat(FILE* input);

/* =============== PUBLIC FUNCTIONS =============== */

SudokuBoard* load(SudokuBoard *sudoku, char *fileName, int *success) {
	int i, j, n, m, N, num;
	FILE *input;

	if (fileName == NULL) { /* Edit without a file */
		if (sudoku != NULL)
			destroyBoard(sudoku); /* Free memory resources of old board */
		sudoku = initializeBoard(3, 3);
		*success = 1;
		return sudoku;
	}

	if (access(fileName, F_OK) == -1) {
		printError(FILE_NOT_EXIST);
		*success = 0;
		return sudoku;
	}

	input = fopen(fileName, "r");
	if (!assertFopen(input)) {
		printError(FILE_UNHANDLED);
		*success = 0;
		return sudoku;
	}

	if (!isValidBoardFormat(input)) {
		printError(INVALID_BOARD_FORMAT);
		fclose(input);
		*success = 0;
		return sudoku;
	}

	if (sudoku != NULL)
		destroyBoard(sudoku); /* Free memory resources of old board */

	rewind(input);
	fscanf(input, "%d", &m);
	fscanf(input, "%d", &n);
	sudoku = initializeBoard(n, m);
	N = n * m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			fscanf(input, "%d", &num);
			getCell(sudoku, i, j)->value = num;
			if (fgetc(input) == '.') {
				getCell(sudoku, i, j)->fixed = 1;
			}
		}
	}

	markErroneousCells(sudoku);

	fclose(input);
	*success = 1;
	return sudoku;
}

void save(SudokuBoard *sudoku, char *fileName, Status mode) {
	int i, j, n, m, c, N;
	FILE *output;

	if (mode == EDIT) {
		if (isErroneous(sudoku)) {
			printError(ERRONEOUS_BOARD);
			return;
		}
		if ((c = validate(sudoku)) != 1) {
			if (c == 0) {
				printError(UNSOLVEABLE_BOARD);
			}
			if (c == -1) {
				printError(UNABLE_TO_VERIFY);
			}
			return;
		}
	}

	output = fopen(fileName, "w");
	if (!assertFopen(output)) {
		printError(WRONG_PATH);
		return;
	}

	m = sudoku->m, n = sudoku->n;
	fprintf(output, "%d %d", m, n);
	N = n * m;
	for (i = 0; i < N; i++) {
		fprintf(output, "\n");
		for (j = 0; j < N; j++) {
			fprintf(output, "%d", getCell(sudoku, i, j)->value);
			if (mode == EDIT || getCell(sudoku, i, j)->fixed == 1) {
				if (getCell(sudoku, i, j)->value != 0) {
					fprintf(output, ".");
				}
			}
			if (j != N - 1) {
				fprintf(output, " ");
			}
		}
	}
	fprintf(output, "\n");
	fclose(output);
	printInstruction(GAME_SAVED);
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Returns the length of the longest word in the file, starting to read from where the pointer is.
 * @pre - file is open.
 */
int longestWord(FILE *input) {
	int c, length = 0;
	int longest = 0;
	while ((c = fgetc(input)) != -1) {
		assertFget();
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

/*
 * Checks if a word represents a valid cell format, i.e. number is in the range 0-N (board dimension), possibly with dot at the end.
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
			return 0;
		}
		i++;
	}

	x = atoi(word); /* Will ignore the dot, if exists */

	if (x == 0 && fixed == 1) { /* Empty cell cannot be fixed, and a dot without a number before is also illegal */
		return 0;
	}

	if (x >= 0 && x <= N) {
		return 1;
	}

	return 0;
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
 * Returns 1 iff 'input' is a valid path to a file with a sudoku board in the correct format.
 * @pre - input is a pointer to a file in read mode.
 */
int isValidBoardFormat(FILE* input) {
	int debug = 0;
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

	if (fscanf(input, "%s", stream) == -1 || !isValidNumber(stream, MAX_M)) { /* Reached EOF while reading n, or invalid number */
		free(stream);
		if (debug)
			printf("m is erroneous\n");
		return 0;

	}

	m = atoi(stream);

	if (fscanf(input, "%s", stream) == -1 || !isValidNumber(stream, MAX_N)) { /* Reached EOF while reading n, or invalid number */
		free(stream);
		if (debug)
			printf("n is erroneous\n");
		return 0;

	}

	n = atoi(stream);

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
