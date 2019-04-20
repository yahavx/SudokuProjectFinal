/*
 * LPSolver.h:
 * This module is used to solve a sudoku board with LP or with ILP.
 *
 * It supplies an interface that allows the user to receive the assignment received for
 * each variable through the LP, as well as information to which (i,j,v)'s weren't assigned
 * a variable (where i is a row index, j is a column index, and v is a value),
 * and why (cell contained a value before, or v is illegal for the cell).
 */

#ifndef LPSOLVER_H_
#define LPSOLVER_H_

#include "StandardLinkedList.h"

/*
 * LPSolution is used to hold the information received from the LP solver.
 * It should be able to store and return the assignment for each variable.
 
 * Also, for each cell (i,j) with value v which wasn't assigned a variable, it
 * should hold the information whether (i,j) wasn't empty, or it was empty but
 * value v was illegal for the cell.
 * 
 * If a change in the board is made after creating it (via the LP solver), the stored information may be no longer relevant.
 */
typedef struct _LPSolution LPSolution;

/*
 * Solves the board using linear programming. Returns a LPSolution struct with the solution information.
 * @param integerSolution - determines whether the solver will use LP (0) or ILP (1).
 */
LPSolution* getLPSolution(SudokuBoard *sudoku, int integerSolution);

/*
 * Returns the assignment of cell (i,j) with value v (i.e. variable (i,j,v)), received via LP.
 * If (i,j,v) doesn't have a variable:
 * - If it's because cell (i,j) already contains a value, returns -1.
 * - If it's because value v is illegal for cell (i,j), in respect to current board state, returns -2.
 */
double getVariableAssignment(LPSolution *boardSol, int i, int j, int v);

/*
 * Returns 1 if the board is solveable, 0 if unsolveable, and -1 if LP solver encountered an error.
 */
int getSolutionStatus(LPSolution *boardSol);

/*
 * Destroys boardSol and free it's memory resources.
 */
void destroyLPSolution(LPSolution *boardSol);



/* ================== MOVE TO OTHER MODULES ================== */

/*
 * Solves the board and prints the assignment of cell (i,j) in the solution.
 * If the board is unsolveable, or the cell is not empty, prints an error and returns 0 (otherwise 1).
 */
int hint(SudokuBoard *sudoku, int i, int j);

/*
 * Performs the 'guess_hint' operation on cell (i,j).
 * Prints the list of legal moves of the cell with a score greater than 0.
 */
void guessHint(SudokuBoard *sudoku, int i, int j);

/*
 * Validate the board using linear programming.
 * Returns 1 if the board is solveable, 0 if unsolveable, and -1 if an error encountered.
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
 * Returns 1 if the command was successful, otherwise 0 and prints an error.
 */
int generate(SudokuBoard *sudoku, int X, int Y, List *l);

/* ======== TEST FUNCTIONS (remove before submitting) ============ */



LPSolution* initializeLPSolution(int N);

int getN(LPSolution *boardSol);

StandardList **getLists(LPSolution *boardSol);

/* =============== PRIVATE FUNCTIONS =============== */

void addVariable(LPSolution *boardSol, int i, int j, int v, int index);
int getVariableIndex(LPSolution *boardSol, int i, int j, int v);
void getSolution(SudokuBoard *sudoku);

#endif /* GUROBI_H */
