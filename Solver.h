/*
 * Solver.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef SOLVER_H_
#define SOLVER_H_

#include "Game.h"

/*
 * Checks whether the current configuration of the board is valid - with respect to index (i,j).
 */
int validConfiguration(SudokuBoard *sudoku, int i, int j);

/*
 * Check if neighbours of (i,j) with value v are erroneous.
 * Mark them accordingly.
 */
void validNeighbours(SudokuBoard* sudoku, int i, int j, int val);

/*
 * Increment the index (i,j), and returns the i of the new index.
 * @param N - board dimension.
 */
int nextI(int i, int j, int N);

/*
 * Increment the index (*,j), and returns the j of the new index.
 * @param N - board dimension.
 */
int nextJ(int j, int N);

/*
 * Returns the number of solutions to the board, -1 if the board is erroneous.
 */
int findNumberOfSolutions(SudokuBoard* sudoku);

/*
 * Assigns a solution found via LP to the board.
 * @pre - board is solveable.
 */
void getSolution(SudokuBoard *sudoku);

#endif /* SOLVER_H_ */
