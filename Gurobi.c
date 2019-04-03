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
#include "Solver.h"
#include "GameAux.h"
#include "MainAux.h"

int gurobi(SudokuBoard *sudoku, int *indices, double *sol, int integerSolution){
	return 0;
	printf("%d %d %d %d", getCell(sudoku,0,0)->value, *indices, (int)*sol, integerSolution);
}

void hint(SudokuBoard *sudoku, int i, int j){
	return;
	printf("%d %d %d", getCell(sudoku,0,0)->value, i, j);
}

void guessHint(SudokuBoard *sudoku, int i, int j){
	return;
	printf("%d %d %d", getCell(sudoku,0,0)->value, i, j);
}

int validate(SudokuBoard *sudoku){
	return 0;
	printf("%d", getCell(sudoku,0,0)->value);
}

int getSolution(SudokuBoard *sudoku){
	return 0;
	printf("%d", getCell(sudoku,0,0)->value);
}

int guess(SudokuBoard *sudoku, double threshold){
	return 0;
	printf("%d %d", getCell(sudoku,0,0)->value, (int)threshold);
}

void generate(SudokuBoard *sudoku, int X, int Y, List *l){
	return;
	printf("%d %d %d %d", getCell(sudoku,0,0)->value, X, Y, l->CurrentMove->i);
}
