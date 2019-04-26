/*
 * GameAux.c:
 * This module implements GameAux.h.
 */

#include <stdio.h>
#include <stdlib.h>
#include "Game.h"
#include "Solver.h"
#include "LPSolver.h"

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

int getRandomIndex(int *legals, int i, int j, LPSolution *boardSol,
		int legalNumbers, double *candidates) {
	int k;
	double random, accumulativeSum = 0, sum = 0;

	if (legalNumbers == 1) { /* One legal number for the cell */
		return 0; /* His index is 0 */
	}

	for (k = 0; k < legalNumbers; k++) {
		candidates[k] = getVariableAssignment(boardSol, i, j, legals[k]);

	}

	for (k = 0; k < legalNumbers; k++) {
		sum += candidates[k];
	}

	if (sum == 0) { /* All cell variables assigned zero */
		random = rand() % legalNumbers; /* Pick one randomly */
		return random;
	}

	for (k = 0; k < legalNumbers; k++) { /* Make probabilities sum equal to 1 */
		candidates[k] = candidates[k] / sum;
	}

	random = ((double) rand()) / ((double) (RAND_MAX) + 1); /* draw a number from 0 to 1 */

	for (k = 0; k < legalNumbers; k++) { /* Draw an index according to probability */
		accumulativeSum += candidates[k]; /* Using accumulative sum method */
		if (random < accumulativeSum) {
			return k;
		}
	}
	return -1; /* Shouldn't reach this line */
}

void addChangesToList(SudokuBoard *sudoku, SudokuBoard *originalSudoku, List *l) {
	int i, j, oldValue, newValue, first = 1, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			newValue = getCell(sudoku, i, j)->value;
			oldValue = getCell(originalSudoku, i, j)->value;
			if (oldValue != newValue) {
				if (first) {
					addNewMove(l, oldValue, newValue, i, j, 1, 0); /* continue backwards of the first is 0 */
					first = 0;
				} else
					addNewMove(l, oldValue, newValue, i, j, 1, 1);
			}
		}
	}

	l->CurrentMove->continueForward = 0; /* continue forward of the last is 0 */
}

int getLegalMovesGuess(SudokuBoard *sudoku, int i, int j, int *legals,
		LPSolution *boardSol, double threshold) {
	int v, legalNumbers = 0, N = sudoku->n * sudoku->m;

	for (v = 1; v <= N; v++) {
		if (getVariableAssignment(boardSol, i, j, v) >= threshold) { /* Value v has a high enough score */

			getCell(sudoku, i, j)->value = v;
			if (validConfiguration(sudoku, i, j)) { /* And is also legal according to current board state */
				legals[legalNumbers++] = v;
			}
		}
	}

	getCell(sudoku, i, j)->value = 0; /* clear the cell back */

	return legalNumbers;
}

int getLegalMoves(SudokuBoard *sudoku, int i, int j, int *legals) {
	int v, legalNumbers = 0, N = sudoku->n * sudoku->m;

	for (v = 1; v <= N; v++) {
		getCell(sudoku, i, j)->value = v;
		if (validConfiguration(sudoku, i, j)) {
			legals[legalNumbers++] = v;
		}
	}

	getCell(sudoku, i, j)->value = 0; /* Clear the cell back */

	return legalNumbers;
}

int emptyCellsNum(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m, count = 0;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->value == 0) {
				count++;
			}
		}
	}

	return count;
}

void clearUnfixedCells(SudokuBoard *sudoku) {
	int i, j, N = sudoku->n * sudoku->m;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->fixed == 0) {
				getCell(sudoku, i, j)->value = 0;
			}
		}
	}
}
