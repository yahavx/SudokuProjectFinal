/*
 * MainAux.c
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "MainAux.h"
#include "GameAux.h"

void assertMalloc(void* pointer) {
	if (pointer == NULL) {
		printError(MALLOC_FAILED);
		exit(0);
	}
}

void initCell(Cell *c, int data) {
	c->error = 0;
	c->fixed = 0;
	c->value = data;
}

SudokuBoard* initializeBoard(int n, int m) {
	int i, N;
	SudokuBoard *sudoku = (SudokuBoard*) malloc(sizeof(SudokuBoard));
	assertMalloc((void*) (sudoku));
	sudoku->n = n, sudoku->m = m;

	N = n * m;
	sudoku->board2 = (Cell*) malloc(N * N * sizeof(Cell));
	assertMalloc((void*) (sudoku->board2));

	for (i = 0; i < N * N; i++) {
		initCell(&(sudoku->board2[i]), 0);
	}
	return sudoku;

}

void destroyBoard(SudokuBoard* sudoku) {
	free(sudoku->board2);
	free(sudoku);
	sudoku = NULL;
}

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
void printBoard3(SudokuBoard *sudoku) {
	printBoard(sudoku, SOLVE, 1);
}

/*
 void printAction(Action actionType){

 }
 */

void printInstruction(Instruction instType){

	if (instType == WIN){
		printf("Congratulations! You solved the puzzle.\n");
	}

	if (instType == SOLVEABLE){
		printf("Board is solveable.\n");
	}

	if (instType == UNSOLVEABLE){
		printf("Board is unsolveable.\n");
	}

	if (instType == EXITING){
		printf("Hope you enjoyed the game! Exiting...\n");
	}

	if (instType == WELCOME){
		printf("Welcome to the sudoku program! Enter a command to begin.\n");
	}
}

void printError(Error errorType) {

	/* errors releated to legal commands */

	if (errorType == FIXED_CELL) {
		printf("Error: cell is fixed.\n");
	}

	if (errorType == ERRONEOUS_BOARD) {
		printf("Error: board is erroneous.\n");
	}

	if (errorType == NO_MOVE_TO_UNDO) {
		printf("Error: there is no move to undo.\n");

	}
	if (errorType == NO_MOVE_TO_REDO) {
		printf("Error: there is no move to redo.\n");
	}

	if (errorType == FILE_NOT_EXIST) {
		printf("Error: the file does not exist.\n");
	}

	if (errorType == INVALID_BOARD_FORMAT) {
		printf("Error: board format is invalid.\n");
	}

	if (errorType == UNSOLVEABLE_BOARD) {
		printf("Error: board is unsolveable.\n");
	}

	if (errorType == CELL_IS_NOT_EMPTY){
		printf("Error: cell already contains a value.\n");
	}

	/* errors releated to wrong input */

	if (errorType == TOO_MANY_PARAMS) {
		printf("Error: not enough parameters.\n");
	}

	if (errorType == NOT_ENOUGH_PARAMS) {
		printf("Error: not enough parameters.\n");
	}

	if (errorType == AVAILABLE_IN_EDIT_AND_SOLVE) {
		printf(
				"Error: this command is available only in Solve and Edit modes.\n");
	}

	if (errorType == AVAILABLE_IN_EDIT) {
		printf("Error: this command is available only in Edit mode.\n");
	}

	if (errorType == AVAILABLE_IN_SOLVE) {
		printf("Error: this command is available only in Solve mode.\n");
	}

	/* system errors */

	if (errorType == FILE_UNHANDLED){
		printf("Error: couldn't handle file.\n");
	}

	if (errorType == FGET_FAILED){
		printf("Error: fgets has failed.\n");
	}

	if (errorType == MALLOC_FAILED){
		printf("Error: malloc has failed. Terminating the program...\n");
	}

	if (errorType == GUROBI_FAILED){
		printf("Error: gurobi has encountered an error.\n");
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

void initializeArray(char c[], int N) {
	int i;
	for (i = 0; i < N; i++) {
		c[i] = '\0';
	}
}

