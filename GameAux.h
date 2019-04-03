/*
 * GameAux.h:
 * This module is an auxiliary module for Game.
 *
 *  Created on: 26 áôáø× 2019
 *      Author: Yahav
 */

#ifndef GAMEAUX_H_
#define GAMEAUX_H_

#include "Game.h"

/*
 * Returns a pointer to the cell (i,j) of sudoku.
 */
Cell* getCell(SudokuBoard *sudoku, int i, int j);

/*
 * Returns 1 iff the board contains an erroneous cell.
 */
int isErroneous(SudokuBoard *sudoku);

/*
 * @pre - cell (i,j) is empty
 * Checks if cell (i,j) contains a single legal value.
 * Returns the value if yes, 0 otherwise. Sets cell to zero when finish.
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

/*
 * Returns 1 iff the board is solved.
 */
int isSolved(SudokuBoard *sudoku);

/*
 * Returns 1 iff the board is full (i.e. all cells are filled with a value from 1 to N).
 */
int isFull(SudokuBoard *sudoku);

#endif /* GAMEAUX_H_ */
