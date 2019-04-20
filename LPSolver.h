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
 * It stores and later allow to receive the assignment for each variable.
 
 * Also, for each cell (i,j) with value v which wasn't assigned a variable, it
 * holds the information whether (i,j) wasn't empty, or it was empty but
 * value v was illegal for the cell.
 * 
 * If a change in the board is made after creating it (via the LP solver), the stored information may be no longer relevant.
 */
typedef struct {
	int N;
	StandardList **mapping;
	double *sol;
	int foundSolution;
} LPSolution;

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

/* ======== TEST FUNCTIONS (remove before submitting) ============ */

LPSolution* initializeLPSolution(int N);

int getN(LPSolution *boardSol);

StandardList **getLists(LPSolution *boardSol);

void addVariable(LPSolution *boardSol, int i, int j, int v, int index);

int getVariableIndex(LPSolution *boardSol, int i, int j, int v);

#endif /* GUROBI_H */
