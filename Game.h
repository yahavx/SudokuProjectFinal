/*
 * Game.h:
 * This moudle implements most of the game operations:
 * set, validate, guess, generate, undo, redo, hint, guess_hint, reset, autofill.
 *
 * It also defines the sudoku struct and implementation.
 */

#ifndef GAME_H_
#define GAME_H_

#include "LinkedMovesList.h"

/* Defines game status */
typedef enum {
	INIT, EDIT, SOLVE
} Status;

/*
 * This struct represents a cell in the board.
 * value contains the value of the cell, 0 if it is empty.
 * fixed == 1 iff the cell is fixed.
 * error == 1 iff the cell value is erroneous in respect to it's containing board.
 */
typedef struct {
	int value;
	int fixed;
	int error;
} Cell;

/*
 * This struct represents a sudoku board.
 * board is the board reprenstation (an array of cells).
 * n, m are the board dimensions, the board contains nxm blocks, each of size mxn.
 *
 * The cell (i,j) is represented by board[i*N+j], where N=n*m.
 */
typedef struct {
	int n, m;
	Cell *board;

} SudokuBoard;

/*
 * Sets cell (i,j) value to z. Mark cell as erroneous is needed, aswell for neighbours that were affected.
 * If cell is fixed and in solve mode, prints an error and returns 0 (otherwise 1).
 */
int set(SudokuBoard* sudoku, int i, int j, int z, Status mode, List* list);

/*
 * Validate the board using linear programming.
 * Returns 1 if the board is solveable, 0 if unsolveable, and -1 if an error encountered.
 * If board is erroneous, a message is printed.
 */
int validate(SudokuBoard *sudoku);

/*
 * Peforms the 'guess' operation with the parameter threshold.
 * Returns 1 if the command was successfull.
 */
int guess(SudokuBoard *sudoku, double threshold, List *l);

/*
 * Peforms the 'generate' operation.
 * @params X - number of cells to fill in the current board, Y - number of cells to keep after the board is solved.
 * Returns 1 if the command was successful, otherwise 0 and prints appropriate error.
 */
int generate(SudokuBoard *sudoku, int X, int Y, List *l);

/*
 * Performs an undo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Returns 1 on success, 0 otherwise.
 */
int undo(SudokuBoard* sudoku, List * list);

/*
 * Performs a redo operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Returns 1 on success, 0 otherwise.
 */
int redo(SudokuBoard* sudoku, List * list);

/*
 * Solves the board and prints the assignment of cell (i,j) in the solution.
 * If the board is unsolveable, or the cell is not empty, prints an error and returns 0 (otherwise 1).
 */
void hint(SudokuBoard *sudoku, int i, int j);

/*
 * Performs the 'guess_hint' operation on cell (i,j).
 * Prints the list of legal moves of the cell with a score greater than 0.
 */
void guessHint(SudokuBoard *sudoku, int i, int j);

/* Performs a reset operation on the board.
 * @params: pointer to undo/redo list and pointer to the board.
 * Prints the board afterwards.
 */
int reset(SudokuBoard* sudoku, List * list);

/*
 * Fill all cells which contains a single legal value.
 * Returns 1 iff action is performed successfully.
 */
int autofill(SudokuBoard* sudoku, List *l);

#endif /* GAME_H_ */
