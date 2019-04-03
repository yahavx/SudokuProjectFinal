/*
 * Gurobi.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef GUROBI_H_
#define GUROBI_H_

int gurobi(SudokuBoard *sudoku, int *indices, double *sol, int integerSolution);

/*
 * 
 */
int hint(SudokuBoard *sudoku, int i, int j);

/*
 * 
 */
void guessHint(SudokuBoard *sudoku, int i, int j);

/*
 * 
 */
int validate(SudokuBoard *sudoku);

/*
 * 
 */
int getSolution(SudokuBoard *sudoku);

/*
 * Peforms the 'guess' operation with the parameter threshold.
 * Returns 1 if the command was successfull.
 */
int guess(SudokuBoard *sudoku, double threshold);

/*
 * Peforms the 'generate' operation.
 * @params X - number of cells to fill in the current board, Y - number of cells to keep after the board is solved.
 * Returns 1 if the command was successfull, 0 otherwise.
 */
int generate(SudokuBoard *sudoku, int X, int Y, List *l);

#endif /* GUROBI_H */