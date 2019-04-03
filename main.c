/*
 * main.c
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "MainAux.h"
#include "Parser.h"
#include "GameAux.h"
#include "Solver.h"
#include "test.h"
#include "SPBufferset.h"
#include "Gurobi.h"

int main() {
	SudokuBoard *sudoku = NULL, *sudokuAux = NULL;
	Status mode = INIT;
	Command *c;
	int markErrorsVar = 1, temp, print = 0, exit = 0;
	List *l = createNewList();
	SP_BUFF_SET()

	printInstruction(WELCOME);

	while (!exit) {
		c = parseInput(sudoku, mode);
		if (1) printf("%d %d %d %d %s %f\n",c->command, c->params[0], c->params[1], c->params[2], c->path, c->threshold);
		switch (c->command) { /* c.command is command type */
		case COMMAND_TOO_LONG:
		case UNKNOWN_COMMAND:
		case ILLEGALY_HANDLED_COMMAND:
		case EMPTY_COMMAND:
			break; /* appropriate errors/instructions printed through parser */

		case SOLVE_COMMAND:
			sudokuAux = load(sudoku, c->path);
			if (sudokuAux != sudoku) { /* load was successful */
				sudoku = sudokuAux;
				mode = SOLVE;
				destroyFromCurrent(l->Head); /* clear moves list */
				printBoard(sudoku, mode, markErrorsVar);
			}
			break;

		case EDIT_WITH_FILE_NAME:
		case EDIT_WITHOUT_FILE_NAME:
			if (c->command == EDIT_WITH_FILE_NAME) {
				sudokuAux = load(sudoku, c->path);
			} else { /* edit without file name */
				sudokuAux = load(sudoku, NULL);
			}
			if (sudokuAux != sudoku) { /* load was successful */
				sudoku = sudokuAux;
				mode = EDIT;
				destroyFromCurrent(l->Head); /* clear moves list */
				print = 1;
			}
			break;

		case MARK_ERRORS:
			markErrorsVar = c->params[0];
			break;

		case PRINT_BOARD:
			print = 1;
			break;

		case SET:
			print = set(sudoku, c->params[1], c->params[0], c->params[2], mode,
					l);
			if (mode == EDIT) {
				if (isSolved(sudoku)) {
					printInstruction(WIN);
					mode = INIT;
				} else if (isFull(sudoku)) {
					printError(ERRONEOUS_BOARD);
				}
			}
			break;

		case VALIDATE:
			if ((temp = validate(sudoku)) == 1) {
				printInstruction(SOLVEABLE);
			} else if (temp == 0) {
				printInstruction(UNSOLVEABLE);
			}
			break;

		case GUESS:
			print = guess(sudoku, c->threshold);
			break;

		case GENERATE:
			print = generate(sudoku, c->params[0], c->params[1], l);
			break;

		case UNDO:
			print = undo(sudoku, l);
			break;

		case REDO:
			print = redo(sudoku, l);
			break;

		case SAVE:
			save(sudoku, c->path, mode);
			break;

		case HINT:
			hint(sudoku, c->params[1],c->params[0]);
			break;

		case GUESS_HINT:
			guessHint(sudoku, c->params[1], c->params[0]);
			break;

		case NUM_SOLUTIONS:
			findNumberOfSolutions(sudoku);
			break;

		case AUTOFILL:
			print = autofill(sudoku,l);
			break;

		case RESET:

		case EXIT:
			exit = 1;
			break;
		}
		free(c);
		if (print) {
			printBoard(sudoku, mode, markErrorsVar);
			print = 0;
		}
	}

	printInstruction(EXIT);
	return 1;
}

