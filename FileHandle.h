/*
 * FileHandle.h:
 * This module contains functions that support loading and saving a board, i.e. solve, edit, save.
 *
 *  Created on: 16 באפר׳ 2019
 *      Author: yahav
 */

#ifndef FILEHANDLE_H_
#define FILEHANDLE_H_

/*
 * Loads a board from <fileName>, if it is in the correct format.
 * If there is an error (i.e. invalid file or invalid board format), returns the input board.
 * Used for solve and edit commands.
 */
SudokuBoard* load(SudokuBoard *sudoku, char *fileName, int *success);

/*
 * Saves a board to <fileName>.
 * @pre - if in edit mode, board is not erroneous and has a solution.
 */
void save(SudokuBoard *sudoku, char *fileName, Status mode);

#endif /* FILEHANDLE_H_ */
