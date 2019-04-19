/*
 * FileHandle.h:
 * This module contains functions that support loading and saving a board, i.e. solve, edit, save.
 */

#ifndef FILEHANDLE_H_
#define FILEHANDLE_H_

/*
 * Loads a board from <fileName>, if it is in the correct format.
 * If the file or board format (in the file) are invalid, prints appropriate message
 * and returns the original board (@param sudoku).
 * Used for solve and edit commands.
 */
SudokuBoard* load(SudokuBoard *sudoku, char *fileName, int *success);

/*
 * Saves a board to <fileName>.
 * If the file path is invalid or the board has no solution and in edit mode, prints appropriate
 * message and exits without saving.
 */
void save(SudokuBoard *sudoku, char *fileName, Status mode);

#endif /* FILEHANDLE_H_ */
