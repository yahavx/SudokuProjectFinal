/*
 * GameAux.c
 *
 *  Created on: 26 áôáø× 2019
 *      Author: Yahav
 */

#include "Game.h"
#include "Solver.h"

Cell* getCell(SudokuBoard *sudoku, int i, int j) {
	int N = sudoku->n * sudoku->m;
	return &(sudoku->board[i * N + j]);
}

int isSingleLegalValue(SudokuBoard *sudoku, int i, int j) {
	int k, N = sudoku->n * sudoku->m;
	int found = 0, legalNum = 0;

	for (k = 1; k <= N; k++) {
		getCell(sudoku, i, j)->value = k;

		if (validConfiguration(sudoku, i, j)) {
			if (found == 1) { /* second legal value */
				getCell(sudoku, i, j)->value = 0;
				return 0;
			}

			found = 1;
			legalNum = k;
		}
	}
	getCell(sudoku, i, j)->value = 0;
	return legalNum;
}

void fixCells(SudokuBoard* sudoku, int fixed) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->value != 0) {
				getCell(sudoku, i, j)->fixed = fixed;
			}
		}
	}
}

int isErroneous(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->error == 1) {
				return 1;
			}
		}
	}
	return 0;
}

void markErroneousCells(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			getCell(sudoku, i, j)->error = !validConfiguration(sudoku, i, j); /* If validConfiguration is 0, error should be 1. */
		}
	}
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
