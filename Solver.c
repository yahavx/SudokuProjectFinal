/*
 * Solver.c
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#include "MainAux.h"
#include "Stack.h"
#include "GameAux.h"
#include "Game.h"
#include "Gurobi.h"

/*int validConfiguration2(SudokuBoard* sudoku, int i, int j) {
	int k, l, N, val, n, m;
	Block b;

	n = sudoku->n;
	m = sudoku->m;
	N = n * m;
	val = getCell(sudoku, i, j)->value;
	if (val == 0) {
		return 1;
	}

	for (k = 0; k < N; k++) {  row check
		if ((k != j) && (val == getCell(sudoku, i, k)->value)) {
			return 0;
		}
	}

	for (l = 0; l < N; l++) {  column check
		if ((l != i) && (val == getCell(sudoku, l, j)->value)) {
			return 0;
		}
	}

	b = sudoku->board[i / m][j / n];  the block containing cell (i,j)

	for (k = 0; k < m; k++) {
		for (l = 0; l < n; l++) {
			if (!(i % m == k && j % n == l) && val == b.cells[k][l].value) {
				return 0;
			}
		}
	}

	return 1;
}*/

int validConfiguration(SudokuBoard* sudoku, int i, int j) {
	int k, l, N, val, n, m, blockStartI, blockStartJ;

	n = sudoku->n;
	m = sudoku->m;
	N = n * m;
	val = getCell(sudoku, i, j)->value;
	if (val == 0) {
		return 1;
	}

	for (k = 0; k < N; k++) { /* row check */
		if ((k != j) && (val == getCell(sudoku, i, k)->value)) {
			return 0;
		}
	}

	for (l = 0; l < N; l++) { /* column check */
		if ((l != i) && (val == getCell(sudoku, l, j)->value)) {
			return 0;
		}
	}

	n = sudoku->n;
	m = sudoku->m;
	blockStartI = i - i % m;
	blockStartJ = j - j % n;

	for (k = blockStartI; k < blockStartI + m; k++) { /* block check */
		for (l = blockStartJ; l < blockStartJ + n; l++) {
			if (!(k == i && l == j) && val == getCell(sudoku, k, l)->value) {
				return 0;
			}
		}
	}

	return 1;
}

void validNeighbours(SudokuBoard* sudoku, int i, int j, int val) {
	int k, l, N, n, m, blockStartI, blockStartJ;

	n = sudoku->n;
	m = sudoku->m;
	N = n * m;

	if (i == -1 || j == -1){
		printf("Warning! sentinel sent to validNeighbours.\n");
		return;
	}

	if (val == 0) {
		getCell(sudoku,i,j) -> error = 0;
		return; /* every cell with zero is valid */
	}

	for (k = 0; k < N; k++) { /* row check */
		if (val == getCell(sudoku, i, k)->value) {
			getCell(sudoku,i,k) -> error = 1 - validConfiguration(sudoku,i,k); /* if valid returns 1, error should be 0 */
		}
	}

	for (l = 0; l < N; l++) { /* column check */
		if (val == getCell(sudoku, l, j)->value) {
			getCell(sudoku,l,j) -> error = 1 - validConfiguration(sudoku,l,j);
		}
	}

	n = sudoku->n;
	m = sudoku->m;
	blockStartI = i - i % m;
	blockStartJ = j - j % n;

	for (k = blockStartI; k < blockStartI + m; k++) { /* block check */
		for (l = blockStartJ; l < blockStartJ + n; l++) {
			if (val == getCell(sudoku, k, l)->value) {
				getCell(sudoku,k,l) -> error = 1- validConfiguration(sudoku,k,l);
			}
		}
	}
}

int nextI(int i, int j, int N) {
	if (j == N - 1)
		return i + 1;
	return i;
}

int nextJ(int j, int N) {
	if (j < N - 1) {
		return j + 1;
	}
	return 0;
}

int findDeterministicSolutionRec(SudokuBoard* sudoku, int i, int j) { /* just for tests */

	int num, N = sudoku->n * sudoku->m;

	if (getCell(sudoku, i, j)->fixed == 1) {
		if (validConfiguration(sudoku, i, j)) {
			if (i == N - 1 && j == N - 1) {
				return 1;
			}

			if (findDeterministicSolutionRec(sudoku, nextI(i, j, N),
					nextJ(j, N)) == 1) {
				return 1;
			}
		}
	}

	else {

		for (num = 1; num <= N; num++) {
			getCell(sudoku, i, j)->value = num;

			if (validConfiguration(sudoku, i, j)) {
				if (i == N - 1 && j == N - 1) {
					return 1;
				}

				if (findDeterministicSolutionRec(sudoku, nextI(i, j, N),
						nextJ(j, N)) == 1)
					return 1;
			}
		}

		getCell(sudoku, i, j)->value = 0;
	}

	return 0;
}

int findDeterministicSolutionStack(SudokuBoard* sudoku) {
	int i, j, lastUsed = 0, N, foundSol = 0, backtrack = 0;
	Stack *stk = initialize();

	i = 0, j = 0;
	N = sudoku->n * sudoku->m;

	while (foundSol == 0) {
		if (getCell(sudoku, i, j)->fixed == 1) { /* cell is fixed */
			if (validConfiguration(sudoku, i, j) && lastUsed == 0) {
				if (i == N - 1 && j == N - 1) {
					foundSol = 1;
					break;
				}
				push(i, j, 1, stk); /* use lastUsed as a flag, to backtrack next time */
				i = nextI(i, j, N);
				j = nextJ(j, N);
				continue;
			}

			else { /* no valid configuration, backtrack */
				if (i == 0 && j == 0) {
					break; /* no solution */
				}
				i = top(stk).i;
				j = top(stk).j;
				lastUsed = top(stk).lastUsed;
				pop(stk);
				continue;
			}
		}

		else { /* cell is not fixed */
			do {
				lastUsed++;
				if (lastUsed == N + 1) { /* no legel value for current cell */
					/* set backtrack flag */
					backtrack = 1;
					break;

				}
				getCell(sudoku, i, j)->value = lastUsed;

			} while (!(validConfiguration(sudoku, i, j))); /* increment cell value until legal configuartion is found */

			if (backtrack == 1) { /* backtrack */

				if (i == 0 && j == 0) {
					break; /* can't backtrack, no solution */
				}

				backtrack = 0;
				getCell(sudoku, i, j)->value = 0; /* clear current cell */
				i = top(stk).i;
				j = top(stk).j;
				lastUsed = top(stk).lastUsed;
				pop(stk);
				continue;
			}

			if (i == N - 1 && j == N - 1) { /* found a legal value to the last cell in the board */
				foundSol = 1;
				break;
			}

			push(i, j, lastUsed, stk); /* continue to next cell */
			i = nextI(i, j, N);
			j = nextJ(j, N);
			lastUsed = 0;
			continue;
		}
	}

	return foundSol;
}

int findNumberOfSolutions(SudokuBoard* sudoku) {
	int i, j, lastUsed = 0, N, solutionsCount = 0, backtrack = 0;
	SudokuBoard *sudokuCopy;
	Stack *stk;

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return -1;
	}

	i = 0, j = 0;
	N = sudoku->n * sudoku->m;

	if (N == 1) { /* board is 1x1 */
		return 1;
	}

	if (!validate(sudoku)){
		printf("The board has 0 solutions.\n");
		return 0; /* board is unsolveable */
	}

	stk = initialize();
	sudokuCopy = clone(sudoku);
	fixCells(sudokuCopy, 1);

	while (1) {
		if (getCell(sudokuCopy, i, j)->fixed == 1) { /* cell is fixed */
			if (validConfiguration(sudokuCopy, i, j) && lastUsed == 0) {
				if (i == N - 1 && j == N - 1) {
					solutionsCount++; /* increment solutions counter, and backtrack */
					popAndUpdate(stk, &i, &j, &lastUsed);
					continue;
				}
				push(i, j, 1, stk); /* use lastUsed as a flag, to backtrack next time */
				i = nextI(i, j, N);
				j = nextJ(j, N);
				continue;
			}

			else { /* no valid configuration, backtrack */
				if (i == 0 && j == 0) {
					break; /* can't backtrack, no more solutions */
				}
				popAndUpdate(stk, &i, &j, &lastUsed);
				continue;
			}
		}

		else { /* cell is not fixed */
			do {
				lastUsed++;
				if (lastUsed == N + 1) { /* no legel value for current cell */
					/* set backtrack flag */
					backtrack = 1;
					break;

				}
				getCell(sudokuCopy, i, j)->value = lastUsed;

			} while (!(validConfiguration(sudokuCopy, i, j))); /* increment cell value until legal configuartion is found */

			if (backtrack == 1) { /* backtrack */

				if (i == 0 && j == 0) {
					break; /* can't backtrack, no more solutions */
				}

				backtrack = 0;
				getCell(sudokuCopy, i, j)->value = 0; /* clear current cell */
				popAndUpdate(stk, &i, &j, &lastUsed);
				continue;
			}

			if (i == N - 1 && j == N - 1) { /* found a legal value to the last cell in the board */
				solutionsCount++; /* increment solutions counter, and backtrack */
				/*printBoard(sudokuCopy);*/
				getCell(sudokuCopy, i, j)->value = 0;

				popAndUpdate(stk, &i, &j, &lastUsed);
				continue;
			}

			push(i, j, lastUsed, stk); /* continue to next cell */
			i = nextI(i, j, N);
			j = nextJ(j, N);
			lastUsed = 0;
			continue;
		}
	}

	destroyStack(stk);
	destroyBoard(sudokuCopy);
	printf("The board has %d solutions.\n", solutionsCount);
	return solutionsCount;
}
