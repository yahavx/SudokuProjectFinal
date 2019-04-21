/*
 * Solver.h:
 * This module verifies the board correction and manages the error status of each cell.
 *
 * In addition, allows the user to apply a solution to a board, as well as finding the number of solutions.
 */

#ifndef SOLVER_H_
#define SOLVER_H_

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
 * Returns the number of solutions to the board, -1 if the board is erroneous.
 */
int findNumberOfSolutions(SudokuBoard* sudoku);

/*
 * Assigns a solution found via LP to the board.
 * @pre - board is solveable.
 */
void getSolution(SudokuBoard *sudoku);

#endif /* SOLVER_H_ */
