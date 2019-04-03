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

SudokuBoard* load(SudokuBoard *sudoku, char *fileName) { /* this function implements solve and edit commands */
	int i, j, N, num;
	char n[2], m[2];
	FILE *input;

	if (fileName == NULL) { /* edit without a file */
		if (sudoku != NULL)
			destroyBoard(sudoku); /* free memory resources of old board */
		sudoku = initializeBoard(3, 3);
		return sudoku;
	}

	if (access(fileName, F_OK) == -1) {
		printError(FILE_NOT_EXIST);
		return sudoku;
	}

	input = fopen(fileName, "r");
	if (!assertFopen(input)) {
		return sudoku;
	}

	if (!isValidBoardFormat(input)) {
		printError(INVALID_BOARD_FORMAT);

		fclose(input);
		return sudoku;
	}

	if (sudoku != NULL)
		destroyBoard(sudoku); /* free memory resources of old board */

	rewind(input);
	fscanf(input, "%s", m);
	fscanf(input, "%s", n);
	sudoku = initializeBoard(atoi(n), atoi(m));
	N = atoi(n) * atoi(m);

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

	return sudoku;
}

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
		printf("Undo: changed cell (%d, %d) from %d to %d.\n", curr->i, curr->j,
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
		printf("Redo: changed cell (%d, %d) from %d to %d.\n", curr->i, curr->j,
				curr->oldCellValue, curr->newCellValue);
	} while (list->CurrentMove->continueForward == 1
			&& list->CurrentMove != NULL);

	return 1;
}

void reset(SudokuBoard* sudoku, List * list) {
	if (list->CurrentMove == list->Head) {
		return;
	}

	while (list->CurrentMove != list->Head) { /* while didn't reach the sentinel */
		getCell(sudoku, list->CurrentMove->i, list->CurrentMove->j)->value =
				list->CurrentMove->oldCellValue;
		list->CurrentMove = list->CurrentMove->prev;
	}

	markErroneousCells(sudoku);
}

void save(SudokuBoard *sudoku, char *fileName, Status mode) {
	int i, j, n, m, N;
	FILE *output;

	if (mode == EDIT) {
		if (isErroneous(sudoku)) {
			printError(ERRONEOUS_BOARD);
			return;
		}
		if (!validate(sudoku)) {
			printError(UNSOLVEABLE_BOARD);
			return;
		}
	}

	output = fopen(fileName, "w");
	assertFopen(output);

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
			fprintf(output, " ");
		}
	}
	fclose(output);

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
