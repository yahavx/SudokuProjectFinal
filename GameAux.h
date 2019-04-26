/*
 * GameAux.h:
 * This module is an auxiliary module for Game.
 *
 * In addition, it includes basic board operations and checks.
 *
 * No private functions in this module as they are to be used by other modules.
 */

#ifndef GAMEAUX_H_
#define GAMEAUX_H_

#include "Game.h"
#include "LPSolver.h"

/*
 * Returns a pointer to the cell (i,j) of sudoku.
 */
Cell* getCell(SudokuBoard *sudoku, int i, int j);

/*
 * Returns 1 iff the board contains an erroneous cell.
 */
int isErroneous(SudokuBoard *sudoku);

/*
 * Checks if cell (i,j) contains a single legal value.
 * Returns the value if yes, 0 otherwise.
 * @pre - cell (i,j) is empty.
 */
int isSingleLegalValue(SudokuBoard *sudoku, int i, int j);

/*
 * Fix or unfix all non-zero values in the board.
 * @param fixed - 1 to fix, 0 to unfix.
 */
void fixCells(SudokuBoard* sudoku, int fixed);

/*
 * Mark all erroneous cells in the board.
 */
void markErroneousCells(SudokuBoard *sudoku);

/* ======== Auxiliary functions for game module ======== */

/*
 * Draw a random index, based on probabailies.
 * @params:
 * legal: an array of the legal values for cell (i,j).
 * legalNumbers: the size of legal (i.e. number of valid values).
 * boardSol: a LP solution.
 * candidates: an int array in the size of legal (overwritten).
 */
int getRandomIndex(int *legals, int i, int j, LPSolution *boardSol,
		int legalNumbers, double *candidates);

/*
 * Add to the list all the changes from originalSudoku to sudoku, i.e. every difference in a cell (i,j) is considered a move.
 *
 * originalSudoku is considered to be the old board (i.e. undo-ing will revert to its values).
 */
void addChangesToList(SudokuBoard *sudoku, SudokuBoard *originalSudoku,
		List *list);

/*
 * Get an array of the legal values for cell (i,j)
 * @param legals - an array in the size of N (board dimension), to be overwritten with the legal values.
 *
 * @ret - number of valid values for the cell.
 */
int getLegalMoves(SudokuBoard *sudoku, int i, int j, int *legals);

/*
 * Similar to getLegalMoves, but to be considered legal, a value should be valid for the cell,
 * and also
 */
int getLegalMovesGuess(SudokuBoard *sudoku, int i, int j, int *legals,
		LPSolution *boardSol, double threshold);

/*
 * Returns the number of empty cells in the board.
 */
int emptyCellsNum(SudokuBoard *sudoku);

/*
 * Clears (i.e. set to 0) every cell that is not fixed.
 */
void clearUnfixedCells(SudokuBoard *sudoku);

#endif /* GAMEAUX_H_ */
