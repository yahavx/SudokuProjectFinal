/*
 * LPSolver.c:
 * This module implements LP and ILP using Gurobi.
 */

#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include "Game.h"
#include "Solver.h"
#include "GameAux.h"
#include "MainAux.h"
#include "StandardLinkedList.h"

void printStandardList(StandardList* list);

int getVariableIndex(LPSolution *boardSol, int i, int j, int v);

void addVariable(LPSolution *boardSol, int i, int j, int v, int index);

LPSolution* initializeLPSolution(int N);

void destroyGurobi(GRBenv *env, GRBmodel *model, int *ind, double *val,
		double *obj, char *vtype);

/* =============== PUBLIC FUNCTIONS =============== */

LPSolution* getLPSolution(SudokuBoard *sudoku, int integerSolution) {
	GRBenv *env = NULL;
	GRBmodel *model = NULL;
	LPSolution *boardSol;

	int i, j, v, k, l, n = sudoku->n, m = sudoku->m, N = n * m, index = 0,
			valid;
	int constraints = 0, varsInConstraint, foundValue = 0;
	int error = 0;
	double *sol = NULL;
	int *ind = malloc(N * sizeof(int));
	double *val = malloc(N * sizeof(double));
	double *obj = NULL;
	char *vtype = NULL;
	int optimstatus;
	double objval;

	assertMalloc((void*) ind);
	assertMalloc((void*) val);

	boardSol = initializeLPSolution(N);

	/* Create environment - log file is sudoku.log */
	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		printf("ERROR %d GRBloadenv(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* Create an empty model named "sudoku" */
	error = GRBnewmodel(env, &model, "sudoku", 0, NULL, NULL, NULL, NULL, NULL);
	if (error) {
		printf("ERROR %d GRBnewmodel(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* Add variables */

	/* Determine which variables should be defined */

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getCell(sudoku, i, j)->value != 0) { /* Cell already contains a number */
				continue;
			}

			valid = 0;

			for (v = 1; v <= N; v++) {
				getCell(sudoku, i, j)->value = v;
				if (validConfiguration(sudoku, i, j)) { /* Value v is legal for cell (i,j), assign a variable and save it's index */
					valid = 1; /* Indicates we found a possible value for cell (i,j) */
					addVariable(boardSol, i, j, v, index);
					index++;
				}
			}
			getCell(sudoku, i, j)->value = 0;

			if (!valid) { /* An empty cell without any legal values */
				addVariable(boardSol, i, j, -1, -1); /* To indicate that cell (i,j) is empty (not to mistake with non-empty cells later) */
			}
		}
	}

	sol = malloc(index * sizeof(double));
	vtype = malloc(index * sizeof(char));
	obj = malloc(index * sizeof(double));

	assertMalloc((void*) sol);
	assertMalloc((void*) vtype);
	assertMalloc((void*) obj);

	boardSol->sol = sol;

	if (integerSolution == 1) { /* Target function is zero - just need to find a feasible solution */
		for (i = 0; i < index; i++) {
			obj[i] = 0;
		}
	}

	else {
		for (i = 0; i < index; i++) { /* Assign random weight from 1 to 10 to each variable */
			obj[i] = 1 + rand() % 10;
		}
	}

	/* variable types  */

	if (integerSolution == 1) {
		for (i = 0; i < index; i++) { /* index holds the number of variables */
			vtype[i] = GRB_BINARY;
		}
	}

	else {
		for (i = 0; i < index; i++) {
			vtype[i] = GRB_CONTINUOUS;
		}
	}

	/* add variables to model */
	error = GRBaddvars(model, index, 0, NULL, NULL, NULL, obj, NULL, NULL,
			vtype, NULL);
	if (error) {
		printf("ERROR %d GRBaddvars(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* Change objective sense to maximization */
	error = GRBsetintattr(model, GRB_INT_ATTR_MODELSENSE, GRB_MAXIMIZE);
	if (error) {
		printf("ERROR %d GRBsetintattr(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* update the model - to integrate new variables */

	error = GRBupdatemodel(model);
	if (error) {
		printf("ERROR %d GRBupdatemodel(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* cell constraints (exactly one value) */

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (getVariableIndex(boardSol, i, j, 1) == -1) { /* this cell already contains a number */
				continue;
			}

			varsInConstraint = 0; /* count how many variables participate in each constraint */

			for (v = 1; v <= N; v++) {
				if (getVariableIndex(boardSol, i, j, v) == -2) { /* value v is not available for this cell (e.g. doesn't have a variable) */
					continue;
				}
				ind[varsInConstraint] = getVariableIndex(boardSol, i, j, v);
				val[varsInConstraint] = 1.0;
				varsInConstraint++;
			}

			if (varsInConstraint == 0) {
				/* An empty cell has no legal values, therefore board is unsolveable (constraint of sum of variables on cell (i,j) == 1 can't be fulfilled */
				destroyGurobi(env, model, ind, val, obj, vtype);
				boardSol->foundSolution = 0;
				return boardSol;
			}

			constraints++;

			error = GRBaddconstr(model, varsInConstraint, ind, val, GRB_EQUAL,
					1.0, NULL);
			if (error) {
				printf("ERROR %d 1st GRBaddconstr(): %s\n", error,
						GRBgeterrormsg(env));
				destroyGurobi(env, model, ind, val, obj, vtype);
				return boardSol;
			}
		}
	}

	/* row constraints */

	for (v = 1; v <= N; v++) {
		for (i = 0; i < N; i++) {

			varsInConstraint = 0;

			for (j = 0; j < N; j++) {
				if (getCell(sudoku, i, j)->value == v) {
					foundValue = 1; /* Indicates we found value v in row i, therefore no need to set this constraint */
					break;
				}

				if (getVariableIndex(boardSol, i, j, v) == -1
						|| getVariableIndex(boardSol, i, j, v) == -2) { /* Cell (i,j) already contains a number, or value v is not legal for (i,j) */
					continue;
				}

				ind[varsInConstraint] = getVariableIndex(boardSol, i, j, v);
				val[varsInConstraint] = 1.0;
				varsInConstraint++;

			}

			if (foundValue == 1) {
				foundValue = 0;
				continue;
			}

			if (varsInConstraint == 0) {
				continue;
			}

			constraints++;
			error = GRBaddconstr(model, varsInConstraint, ind, val, GRB_EQUAL,
					1.0, NULL);
			if (error) {
				printf("ERROR %d 1st GRBaddconstr(): %s\n", error,
						GRBgeterrormsg(env));
				destroyGurobi(env, model, ind, val, obj, vtype);
				return boardSol;
			}
		}
	}

	/* column constraints */

	for (v = 1; v <= N; v++) {
		for (j = 0; j < N; j++) {

			varsInConstraint = 0;

			for (i = 0; i < N; i++) {
				if (getCell(sudoku, i, j)->value == v) {
					foundValue = 1; /* Indicates we found value v in column j, therefore no need to set a constraint */
					break;
				}

				if (getVariableIndex(boardSol, i, j, v) == -1
						|| getVariableIndex(boardSol, i, j, v) == -2) { /* Cell (i,j) already contains a number, or value v is not legal for (i,j) */
					continue;
				}

				ind[varsInConstraint] = getVariableIndex(boardSol, i, j, v);
				val[varsInConstraint] = 1.0;
				varsInConstraint++;
			}

			if (foundValue == 1) {
				foundValue = 0;
				continue;
			}

			if (varsInConstraint == 0) {
				continue;
			}

			constraints++;

			error = GRBaddconstr(model, varsInConstraint, ind, val, GRB_EQUAL,
					1.0, NULL);
			if (error) {
				printf("ERROR %d 1st GRBaddconstr(): %s\n", error,
						GRBgeterrormsg(env));
				destroyGurobi(env, model, ind, val, obj, vtype);
				return boardSol;
			}
		}
	}

	/* block constraints */

	for (k = 0; k < n; k++) { /* k and l iterate over blocks, i and j over cells in each block */
		for (l = 0; l < m; l++) {
			for (v = 1; v <= N; v++) {
				varsInConstraint = 0;
				for (i = k * m; i < (k + 1) * m; i++) {
					for (j = l * n; j < (l + 1) * n; j++) {
						if (getCell(sudoku, i, j)->value == v) {
							foundValue = 1; /* Indicates we found value v in block (i,j), therefore no need to set a constraint */
							break;
						}

						if (getVariableIndex(boardSol, i, j, v) == -1
								|| getVariableIndex(boardSol, i, j, v) == -2) { /* Cell (i,j) already contains a number, or value v is not legal for (i,j) */
							continue;
						}

						ind[varsInConstraint] = getVariableIndex(boardSol, i, j,
								v);
						val[varsInConstraint] = 1.0;
						varsInConstraint++;
					}
				}

				if (foundValue == 1) {
					foundValue = 0;
					continue;
				}

				if (varsInConstraint == 0) {
					continue;
				}

				constraints++;
				error = GRBaddconstr(model, varsInConstraint, ind, val,
						GRB_EQUAL, 1.0, NULL);
				if (error) {
					printf("ERROR %d 1st GRBaddconstr(): %s\n", error,
							GRBgeterrormsg(env));
					return boardSol;
				}

			}
		}
	}

	/* Optimize model - need to call this before calculation */
	error = GRBoptimize(model);
	if (error) {
		printf("ERROR %d GRBoptimize(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* Write model to 'sudoku.lp' - this is not necessary but very helpful */
	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		printf("ERROR %d GRBwrite(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* Get solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		printf("ERROR %d GRBgetintattr(): %s\n", error, GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* get the objective -- the optimal result of the function */
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
		destroyGurobi(env, model, ind, val, obj, vtype);
		boardSol->foundSolution = 0;
		/*printf("ERROR %d GRBgettdblattr(): %s\n", error, GRBgeterrormsg(env));*/
		return boardSol;
	}

	/* get the solution - the assignment to each variable */
	/* 3-- number of variables, the size of "sol" should match */
	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, index, sol);
	if (error) {
		printf("ERROR %d GRBgetdblattrarray(): %s\n", error,
				GRBgeterrormsg(env));
		destroyGurobi(env, model, ind, val, obj, vtype);
		return boardSol;
	}

	/* print results */
	/*printf("\nOptimization complete\n");*/

	/* solution found */
	if (optimstatus == GRB_OPTIMAL) {
		boardSol->foundSolution = 1;
	}

	/* no solution found */
	else if (optimstatus == GRB_INF_OR_UNBD) {
		boardSol->foundSolution = 0;
	}
	/* error or calculation stopped */
	else {
		printf("Optimization was stopped early\n");
	}

	/* IMPORTANT !!! - Free model and environment */
	destroyGurobi(env, model, ind, val, obj, vtype);
	boardSol->sol = sol;

	return boardSol;
}

double getVariableAssignment(LPSolution *boardSol, int i, int j, int v) {
	int index = getVariableIndex(boardSol, i, j, v);
	if (index == -1 || index == -2) { /* This indicates (i,j,v) is not a variable. */
		return index;
	}

	return boardSol->sol[index];
}

int getSolutionStatus(LPSolution *boardSol) {
	return boardSol->foundSolution;
}

void destroyLPSolution(LPSolution *boardSol) {
	int i, j, N = boardSol->N;
	StandardList **mapping = boardSol->mapping;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (mapping[i * N + j] != NULL) {
				destroyStandardList(mapping[i * N + j]);
			}
		}
	}

	free(mapping);

	if (boardSol->sol != NULL) {
		free(boardSol->sol);
	}

	free(boardSol);
}

/* =============== PRIVATE FUNCTIONS =============== */

/*
 * Returns the index of the variable that represents cell (i,j) with value v.
 * If (i,j,v) doesn't have a variable:
 * - If cell (i,j) already contains a value, returns -1.
 * - If cell (i,j) with value v is illegal in respect to the board, returns -2.
 */
int getVariableIndex(LPSolution *boardSol, int i, int j, int v) {
	int N = boardSol->N;
	StandardList **mapping = boardSol->mapping;

	if (mapping[i * N + j] == NULL) { /* No list initialized for (i,j), therefore he already contains a number */
		return -1;
	}

	return lookupInStandardList(mapping[i * N + j], v);
}

/*
 * Assigns a variable to cell (i,j) with value v. Stores it's index.
 */
void addVariable(LPSolution *boardSol, int i, int j, int v, int index) {
	int N = boardSol->N;
	if (boardSol->mapping[i * N + j] == NULL) {
		boardSol->mapping[i * N + j] = createNewStandardList();
	}
	addNewStandardMove(boardSol->mapping[i * N + j], v, index);
}

/*
 * Initializes a new LPSolution struct.
 * Returns a pointer to it.
 */
LPSolution* initializeLPSolution(int N) {
	int i, j;
	LPSolution *boardSol = malloc(sizeof(LPSolution));
	assertMalloc((void*) boardSol);

	boardSol->N = N;
	boardSol->mapping = malloc(N * N * sizeof(StandardList*));
	assertMalloc((void*) boardSol->mapping);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			boardSol->mapping[i * N + j] = NULL;
		}
	}
	boardSol->sol = NULL;
	boardSol->foundSolution = -1; /* Indicates an error. Default. */
	return boardSol;
}

/*
 * Destroys a gurobi model and all it's components, frees memory resources.
 */
void destroyGurobi(GRBenv *env, GRBmodel *model, int *ind, double *val,
		double *obj, char *vtype) {
	GRBfreemodel(model);
	GRBfreeenv(env);
	free(ind);
	free(val);
	if (obj != NULL) {
		free(obj);
	}
	if (vtype != NULL) {
		free(vtype);
	}
	/* LPSolution will be freed outside */
}

/* ======== TEST FUNCTIONS (remove before submitting) ============ */

int getN(LPSolution *boardSol) {
	return boardSol->N;
}

StandardList **getLists(LPSolution *boardSol) {
	return boardSol->mapping;
}

void printStandardList(StandardList* list) {
	StandardNode *q = list->Head;
	while (q != NULL) {
		printf("(%d,%d) --> ", q->val, q->varIndex);
		q = q->next;
	}
	printf("NULL\n\n");
}
