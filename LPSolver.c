/* Copyright 2013, Gurobi Optimization, Inc. */

/* This example formulates and solves the following simple sudoku model:

 maximize    x + 3 y + 2 z
 subject to  x + 2 y + 3 z <= 5
 x +   y       >= 1
 x, y, z binary
 */

#include <stdlib.h>
#include <stdio.h>
#include "Game.h"
#include "GameAux.h"
#include "Solver.h"
#include "MainAux.h"
#include "LPSolver.h"
#include "StandardLinkedList.h"

LPSolution* getLPSolution(SudokuBoard *sudoku, int integerSolution) {
	return NULL;
	printf("%d %d", getCell(sudoku, 0, 0)->value, integerSolution);
}

int getSolutionStatus(LPSolution *boardSol) {
	return 0;
	printf("%d", boardSol->N);
}

void destroyLPSolution(LPSolution *boardSol) {
	int i, j, N = boardSol->N;
	StandardList **mapping = boardSol->mapping;

	if (boardSol == NULL) {
		return;
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (mapping[i * N + j] != NULL) {
				destroyStandardList(mapping[i * N + j]);
			}
		}
	}

	free(mapping);
	if (boardSol->sol != NULL) {
		free(boardSol->sol);
	}
	free(boardSol);
}

/* ================================ */

int getVariableIndex(LPSolution *boardSol, int i, int j, int v) {
	int N = boardSol->N;
	StandardList **mapping = boardSol->mapping;

	if (mapping[i * N + j] == NULL) {
		return -1;
	}

	return lookupInStandardList(mapping[i * N + j], v);
}

double getVariableAssignment(LPSolution *boardSol, int i, int j, int v) {
	int index = getVariableIndex(boardSol, i, j, v);
	if (index == -1 || index == -2) {
		return index;
	}

	return boardSol->sol[index];
}

void addVariable(LPSolution *boardSol, int i, int j, int v, int index) {
	int N = boardSol->N;
	if (boardSol->mapping[i * N + j] == NULL) {
		boardSol->mapping[i * N + j] = createNewStandardList();
	}
	addNewStandardMove(boardSol->mapping[i * N + j], v, index);
}

LPSolution* initializeLPSolution(int N) {
	int i, j;
	LPSolution *boardSol = malloc(sizeof(LPSolution));
	boardSol->N = N;
	boardSol->mapping = malloc(N * N * sizeof(StandardList*));
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			boardSol->mapping[i * N + j] = NULL;
		}
	}
	boardSol->sol = NULL;
	return boardSol;
}

int getN(LPSolution *boardSol) {
	return boardSol->N;
}

StandardList **getLists(LPSolution *boardSol) {
	return boardSol->mapping;
}
