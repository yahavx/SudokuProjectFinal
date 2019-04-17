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
#include "FileHandle.h"

int main() {
	SudokuBoard *sudoku = NULL;
	Status mode = INIT;
	Command *c;
	int markErrorsVar = 1, temp, print = 0, exit = 0;
	List *movesList = createNewList();
	int loaded;

	/*destroyList(movesList);
	test();*/

	SP_BUFF_SET()

	printInstruction(WELCOME);

	while (!exit) {
		printInstruction(ENTER_COMMAND);
		c = parseInput(sudoku, mode);
		/*printf(
				"x : %d , y: %d , z : %d , threshold : %f, cmd : %d , path : %s \n",
				c->params[0], c->params[1], c->params[2], c->threshold,
				c->command, c->path);*/
		switch (c->command) { /* c.command is command type */
		case COMMAND_TOO_LONG:
		case UNKNOWN_COMMAND:
		case ILLEGALY_HANDLED_COMMAND:
		case EMPTY_COMMAND:
			break; /* Appropriate errors/instructions printed through parser */

		case SOLVE_COMMAND:
			sudoku = load(sudoku, c->path, &loaded);

			if (loaded) { /* Load was successful */
				mode = SOLVE;
				destroyFromCurrent(movesList->Head); /* clear moves list */
				print = 1;
			}

			break;

		case EDIT_WITH_FILE_NAME:
		case EDIT_WITHOUT_FILE_NAME:
			if (c->command == EDIT_WITH_FILE_NAME) {
				sudoku = load(sudoku, c->path, &loaded);
			} else { /* Edit without file name */
				sudoku = load(sudoku, NULL, &loaded);
			}

			if (loaded) { /* Load was successful */
				mode = EDIT;
				resetList(movesList); /* clear moves list */
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
					movesList);
			if (mode != EDIT) {
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
			print = generate(sudoku, c->params[0], c->params[1], movesList);
			break;

		case UNDO:
			print = undo(sudoku, movesList);
			break;

		case REDO:
			print = redo(sudoku, movesList);
			break;

		case SAVE:
			save(sudoku, c->path, mode);
			break;

		case HINT:
			hint(sudoku, c->params[1], c->params[0]);
			break;

		case GUESS_HINT:
			guessHint(sudoku, c->params[1], c->params[0]);
			break;

		case NUM_SOLUTIONS:
			findNumberOfSolutions(sudoku);
			break;

		case AUTOFILL:
			print = autofill(sudoku, movesList);
			break;

		case RESET:
			print = reset(sudoku, movesList);
			break;

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

	printInstruction(EXITING);
	destroyList(movesList);
	destroyBoard(sudoku);

	return 0;
}

