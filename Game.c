/*
 * Game.c:
 * This module implements Game.h.
 *
 * It contains the implementations of the funtions declared in the header only.
 * Any auxiliary function is placed in GameAux.
 */

#include <stdlib.h>
#include <stdio.h>
#include "MainAux.h"
#include "Solver.h"
#include "Game.h"
#include "LinkedMovesList.h"
#include "GameAux.h"
#include "LPSolver.h"

/* =============== PUBLIC FUNCTIONS =============== */

int set(SudokuBoard* sudoku, int i, int j, int z, Status mode, List* list) {
	Cell *c = getCell(sudoku, i, j);

	int oldValue;

	if (mode == SOLVE && c->fixed == 1) {
		printError(FIXED_CELL);
		return 0;
	}

	oldValue = getCell(sudoku, i, j)->value;
	addNewMove(list, oldValue, z, i, j, 0, 0);
	c->value = z;

	validNeighbours(sudoku, i, j, oldValue);
	validNeighbours(sudoku, i, j, z); /* Update neighbours error status */

	return 1;
}

int validate(SudokuBoard *sudoku) {
	LPSolution *boardSol;
	int status;
	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return -1;
	}

	boardSol = getLPSolution(sudoku, 1);
	status = getSolutionStatus(boardSol);
	destroyLPSolution(boardSol);
	return status;
}

int guess(SudokuBoard *sudoku, double threshold, List *l) {
	int N = sudoku->n * sudoku->m, legalNumbers, randomIndex;
	int i, j;
	double *candidates;
	LPSolution *boardSol;
	int *legals;
	int changed = 0;
	SudokuBoard *originalSudoku;

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return 0;
	}

	boardSol = getLPSolution(sudoku, 0);

	switch (getSolutionStatus(boardSol)) {
	case -1:
		printError(GUROBI_FAILED);
		break;

	case 0:
		printError(UNSOLVEABLE_BOARD);
		break;

	case 1:
		originalSudoku = clone(sudoku);
		legals = malloc(N * sizeof(int));
		assertMalloc((void*) legals);
		candidates = malloc(N * sizeof(double));
		assertMalloc((void*) candidates);

		for (i = 0; i < N; i++) {
			for (j = 0; j < N; j++) {

				if (getVariableAssignment(boardSol, i, j, 1) == -1) { /* index (i,j) already has a value */
					continue;
				}

				legalNumbers = getLegalMovesGuess(sudoku, i, j, legals,
						boardSol, threshold); /* Edits legals */

				if (legalNumbers == 0) {
					continue;
				}

				changed = 1;
				randomIndex = getRandomIndex(legals, i, j, boardSol,
						legalNumbers, candidates);

				getCell(sudoku, i, j)->value = legals[randomIndex];
				printCellUpdate(GUESS, i, j, 0, legals[randomIndex]);
			}
		}
		if (changed) { /* Indicates a cell value was changed */
			addChangesToList(sudoku, originalSudoku, l); /* Update moves list */
		} else {
			printInstruction(NO_CHANGE);
		}

		free(candidates);
		free(legals);
		destroyBoard(originalSudoku);
		destroyLPSolution(boardSol);
		return 1;
	}

	destroyLPSolution(boardSol);
	return 0;
}

int generate(SudokuBoard *sudoku, int X, int Y, List *l) {
	int i, j, N, chosenCells, attemptsLeft = 1000, emptyCells;
	int *legals, legalNumbers, randomIndex;
	SudokuBoard *sudokuCopy, *originalSudoku;

	emptyCells = emptyCellsNum(sudoku);
	if (X > emptyCells) {
		printInvalidParameter(NOT_ENOUGH_EMPTY_CELLS, emptyCells, 0);
		return 0;
	}

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return 0;
	}

	if (!validate(sudoku)) {
		printError(UNSOLVEABLE_BOARD);
		return 0;
	}

	N = sudoku->n * sudoku->m;
	legals = malloc(N * sizeof(int));
	assertMalloc((void*) legals);
	sudokuCopy = initializeBoard(sudoku->n, sudoku->m);

	chosenCells = 0;

	while (attemptsLeft > 0) {
		chosenCells = 0;
		copy(sudoku, sudokuCopy); /* Revert board to it's original state */

		while (chosenCells < X) {

			i = rand() % N;
			j = rand() % N;

			if (getCell(sudokuCopy, i, j)->value != 0) { /* Cell already has a value */
				continue;
			}

			legalNumbers = getLegalMoves(sudokuCopy, i, j, legals);

			if (legalNumbers == 0) {
				break;
			}

			randomIndex = rand() % legalNumbers;
			getCell(sudokuCopy, i, j)->value = legals[randomIndex];
			chosenCells++;
		}

		if (chosenCells == X) {
			if (validate(sudokuCopy)) {
				break;
			}

		}
		attemptsLeft--;

	}

	free(legals);

	if (attemptsLeft == 0) {
		printError(PUZZLE_GENERATOR_FAILED);
		destroyBoard(sudokuCopy);
		return 0;
	}
	originalSudoku = clone(sudoku); /* Keep a copy of the original sudoku */
	copy(sudokuCopy, sudoku);
	destroyBoard(sudokuCopy);

	getSolution(sudoku); /* Apply solution to sudoku */

	fixCells(sudoku, 0); /* Unfix all values */

	chosenCells = 0;

	while (chosenCells < Y) {

		i = rand() % N;
		j = rand() % N;

		if (getCell(sudoku, i, j)->fixed == 0) { /* Not chosen yet (using fix as choose flags) */
			getCell(sudoku, i, j)->fixed = 1;
			chosenCells++;
		}
	}
	clearUnfixedCells(sudoku); /* Remove all unchosen cells */
	fixCells(sudoku, 0); /* Unfix cells (doesn't really matter) */

	addChangesToList(sudoku, originalSudoku, l); /* Update moves list */
	destroyBoard(originalSudoku);
	return 1;
}

int undo(SudokuBoard* sudoku, List * list) {
	Node *curr;
	int backwards;
	if (list->CurrentMove == list->Head) {
		printError(NO_MOVE_TO_UNDO);
		return 0;
	}
	do {
		getCell(sudoku, list->CurrentMove->i, list->CurrentMove->j)->value =
				list->CurrentMove->oldCellValue;
		curr = list->CurrentMove;
		validNeighbours(sudoku, curr->i, curr->j, curr->newCellValue);
		validNeighbours(sudoku, curr->i, curr->j, curr->oldCellValue); /* Update neighbours error status */
		backwards = list->CurrentMove->continueBackwards;
		list->CurrentMove = list->CurrentMove->prev;
		printCellUpdate(UNDO, curr->i, curr->j, curr->newCellValue,
				curr->oldCellValue);
	} while (backwards == 1 && list->CurrentMove != NULL);

	return 1;
}

int redo(SudokuBoard* sudoku, List * list) {
	Node *curr;
	if (list->CurrentMove == list->Tail) {
		printError(NO_MOVE_TO_REDO);
		return 0;
	}
	do {
		list->CurrentMove = list->CurrentMove->next;
		curr = list->CurrentMove;
		getCell(sudoku, curr->i, curr->j)->value = curr->newCellValue;
		validNeighbours(sudoku, curr->i, curr->j, curr->oldCellValue);
		validNeighbours(sudoku, curr->i, curr->j, curr->newCellValue);
		printCellUpdate(REDO, curr->i, curr->j, curr->oldCellValue,
				curr->newCellValue);

	} while (list->CurrentMove->continueForward == 1
			&& list->CurrentMove != NULL);

	return 1;
}

void hint(SudokuBoard *sudoku, int i, int j) {
	int v, N = sudoku->n * sudoku->m;
	LPSolution *boardSol;

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return;
	}

	if (getCell(sudoku, i, j)->fixed == 1) {
		printError(FIXED_CELL);
		return;
	}

	if (getCell(sudoku, i, j)->value != 0) {
		printError(CELL_IS_NOT_EMPTY);
		return;
	}

	boardSol = getLPSolution(sudoku, 1);

	switch (getSolutionStatus(boardSol)) {
	case -1:
		printError(GUROBI_FAILED);
		break;

	case 0:
		printError(UNSOLVEABLE_BOARD);
		break;

	case 1:
		for (v = 1; v <= N; v++) {
			if (getVariableAssignment(boardSol, i, j, v) == 1.0) {
				printInstructionWithParam(HINT, v);
				break;
			}
		}
		break;
	}

	destroyLPSolution(boardSol);
}

void guessHint(SudokuBoard *sudoku, int i, int j) {
	int v, N = sudoku->n * sudoku->m;
	LPSolution *boardSol;

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return;
	}

	if (getCell(sudoku, i, j)->fixed == 1) {
		printError(FIXED_CELL);
		return;
	}

	if (getCell(sudoku, i, j)->value != 0) {
		printError(CELL_IS_NOT_EMPTY);
		return;
	}

	boardSol = getLPSolution(sudoku, 0);

	switch (getSolutionStatus(boardSol)) {
	case -1:
		printError(GUROBI_FAILED);
		break;

	case 0:
		printError(UNSOLVEABLE_BOARD);
		break;

	case 1:
		for (v = 1; v <= N; v++) {
			if (getVariableAssignment(boardSol, i, j, v) > 0) {
				printGuessHintScore(v,
						getVariableAssignment(boardSol, i, j, v));
			}
		}
		break;
	}

	destroyLPSolution(boardSol);
}

int reset(SudokuBoard* sudoku, List * list) {
	if (list->CurrentMove == list->Head) {
		printError(NO_MOVE_TO_UNDO);
		return 0;
	}

	while (list->CurrentMove != list->Head) { /* While didn't reach the sentinel */
		getCell(sudoku, list->CurrentMove->i, list->CurrentMove->j)->value =
				list->CurrentMove->oldCellValue;
		list->CurrentMove = list->CurrentMove->prev;
	}

	markErroneousCells(sudoku);
	return 1;
}

int autofill(SudokuBoard* sudoku, List *l) {
	int i, j, oldValue, newValue, first = 1, legalVal, N = sudoku->n
			* sudoku->m;
	int change = 0;

	SudokuBoard* sudokuCopy;

	if (isErroneous(sudoku)) {
		printError(ERRONEOUS_BOARD);
		return 0;
	}

	sudokuCopy = clone(sudoku);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudokuCopy, i, j)->value != 0) {
				continue;
			}

			legalVal = isSingleLegalValue(sudokuCopy, i, j); /* Returns the single legal value if exists, 0 otherwise */

			if (legalVal != 0) {
				change = 1; /* Indicates a change was made */
				printCellUpdate(AUTOFILL, i, j, 0, legalVal);
				oldValue = getCell(sudoku, i, j)->value;
				newValue = legalVal;
				if (first) {
					addNewMove(l, oldValue, newValue, i, j, 1, 0); /* continueBackwards of the first is 0 */
					first = 0;
				} else
					addNewMove(l, oldValue, newValue, i, j, 1, 1);
				getCell(sudoku, i, j)->value = legalVal;
			}

		}
	}
	if (!change){
		printInstruction(NO_CHANGE);
	}

	l->CurrentMove->continueForward = 0; /* continueForward of the last is 0 */
	markErroneousCells(sudoku);
	destroyBoard(sudokuCopy);
	return 1;
}
