/*
 * main.c:
 * This module runs and manage the sudoku game.
 *
 * General note: all functions treat indices as 0-based.
 *
 * In addition, we treat an index (i,j) as row and column respectively (not vice versa).
 * This assumption applies to all functions and documentations, only the interaction with the user is an exception (i.e. in I/O the first parameter indicates the column).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "MainAux.h"
#include "Parser.h"
#include "GameAux.h"
#include "Solver.h"
#include "FileHandle.h"

int main() {
	SudokuBoard *sudoku = NULL;
	Status mode = INIT;
	Command *c;
	CommandType lastCommand = ILLEGALY_HANDLED_COMMAND;
	int markErrors = 1, temp, print = 0, exit = 0;
	List *movesList = createNewList();
	int loaded;

	srand(time(NULL)); /* For functions that base on randomization */

	printInstruction(WELCOME);

	while (!exit) {
		if (lastCommand != EMPTY_COMMAND) {
			printInstruction(ENTER_COMMAND);
		}
		c = parseInput(sudoku, mode);

		switch (c->command) { /* c.command is command type */
		case COMMAND_TOO_LONG:
		case ILLEGALY_HANDLED_COMMAND:
		case EMPTY_COMMAND:
			break; /* Appropriate errors/instructions printed through parser */

		case SOLVE_COMMAND:
			sudoku = load(sudoku, c->path, &loaded);

			if (loaded) { /* Load was successful */
				mode = SOLVE;
				resetList(movesList); /* Clear moves list */
				print = 1;
				if (validateSolution(sudoku, mode)) {
					printInstruction(FAKE_WIN);
					mode = INIT;
				}
			}
			break;

		case EDIT_WITH_FILE_NAME:
			sudoku = load(sudoku, c->path, &loaded);

			if (loaded) { /* Load was successful */
				mode = EDIT;
				resetList(movesList); /* clear moves list */
				print = 1;
			}
			break;

		case EDIT_WITHOUT_FILE_NAME:
			sudoku = load(sudoku, NULL, &loaded);

			if (loaded) { /* Load was successful */
				mode = EDIT;
				resetList(movesList); /* clear moves list */
				print = 1;
			}

			break;

		case MARK_ERRORS:
			markErrors = c->params[0];
			printInstructionWithParam(MARK_ERRORS, c->params[0]);
			break;

		case PRINT_BOARD:
			print = 1;
			break;

		case SET:
			print = set(sudoku, c->params[1], c->params[0], c->params[2], mode,
					movesList); /* 'set' assumes the first parameter is row, so need to swap */
			if (print) { /* The set operation was successful */
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
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
			print = guess(sudoku, c->threshold, movesList);
			if (print) { /* The guess operation was successful */
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
				}
			}
			break;

		case GENERATE:
			print = generate(sudoku, c->params[0], c->params[1], movesList);
			break;

		case UNDO:
			print = undo(sudoku, movesList);
			if (print) {
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
				}
			}
			break;

		case REDO:
			print = redo(sudoku, movesList);
			if (print) { /* The guess operation was successful */
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
				}
			}
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
			temp = findNumberOfSolutions(sudoku);
			if (temp != -1) /* -1 indicates an error occured */{
				printInstructionWithParam(NUM_SOLUTIONS, temp);
			}
			break;

		case AUTOFILL:
			print = autofill(sudoku, movesList);
			if (print) { /* The autofill operation was successful */
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
				}
			}
			break;

		case RESET:
			print = reset(sudoku, movesList);
			if (print) { /* The guess operation was successful */
				if (validateSolution(sudoku, mode)) { /* Board is solved */
					printInstruction(WIN);
					mode = INIT;
				}
			}
			break;

		case EXIT:
			exit = 1;
			break;
		}

		lastCommand = c->command;
		free(c);
		if (print) {
			printBoard(sudoku, mode, markErrors);
			print = 0;
		}
	}

	printInstruction(EXITING);
	destroyList(movesList);
	destroyBoard(sudoku);

	return 0;
}

