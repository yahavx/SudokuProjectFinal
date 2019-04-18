/*
 * Game.c
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#include "Solver.h"
#include "Game.h"
#include "MainAux.h"
#include "SPBufferset.h"
#include "LinkedMovesList.h"
#include "ParserAux.h"
#include "GameAux.h"
#include "test.h"
#include "unistd.h"
#include "Gurobi.h"

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
	validNeighbours(sudoku, i, j, z);

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
		validNeighbours(sudoku, curr->i, curr->j, curr->oldCellValue);
		backwards = list->CurrentMove->continueBackwards;
		list->CurrentMove = list->CurrentMove->prev;
		printf("Undo: changed cell (%d,%d) from %d to %d.\n", curr->i+1, curr->j+1,
				curr->newCellValue, curr->oldCellValue);
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
		printf("Redo: changed cell (%d,%d) from %d to %d.\n", curr->i+1, curr->j+1,
				curr->oldCellValue, curr->newCellValue);
	} while (list->CurrentMove->continueForward == 1
			&& list->CurrentMove != NULL);

	return 1;
}

int reset(SudokuBoard* sudoku, List * list) {
	if (list->CurrentMove == list->Head) {
		printError(NO_MOVE_TO_UNDO);
		return 0;
	}

	while (list->CurrentMove != list->Head) { /* while didn't reach the sentinel */
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

			legalVal = isSingleLegalValue(sudokuCopy, i, j);

			if (legalVal != 0) {
				oldValue = getCell(sudoku, i, j)->value;
				newValue = legalVal;
				if (first) {
					addNewMove(l, oldValue, newValue, i, j, 1, 0); /* continue backwards of the first is 0 */
					first = 0;
				} else
					addNewMove(l, oldValue, newValue, i, j, 1, 1);
				getCell(sudoku, i, j)->value = legalVal;
			}

		}
	}
	l->CurrentMove->continueForward = 0; /* continue forward of the last is 0 */
	destroyBoard(sudokuCopy);
	return 1;
}
