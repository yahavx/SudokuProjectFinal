/*
 * Parser.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Game.h"

typedef enum {
	COMMAND_TOO_LONG,
	UNKNOWN_COMMAND, /* command does not exist */
	ILLEGALY_HANDLED_COMMAND, /* command is not used properly */
	EMPTY_COMMAND,
	SOLVE_COMMAND, /* commands start here */
	EDIT_WITH_FILE_NAME,
	EDIT_WITHOUT_FILE_NAME,
	MARK_ERRORS,
	PRINT_BOARD,
	SET,
	VALIDATE,
	GUESS,
	GENERATE,
	UNDO,
	REDO,
	SAVE,
	HINT,
	GUESS_HINT,
	NUM_SOLUTIONS,
	AUTOFILL,
	RESET,
	EXIT

}CommandType;

typedef struct {
	CommandType command;
	int params[3];
	char *path;
	double threshold;
} Command;


/*
 * This struct is used to pass a command with it's parameters from the parser.
 * command is the command type, params are integer parameters (up to 3), and path is for commands that load/save puzzles.
 */


/*
 * Checks if fgets or fgetc has failed. If indeed, prints a message and terminates the program.
 */
void assertFget();

/*
 * Checks if fopen has failed. If indeed, prints a message and terminates the program.
 */
int assertFopen(FILE *fp);

/*
 * Receives and processes input from the user.
 */
Command* parseInput(SudokuBoard* sudoku , Status mode);


#endif /* PARSER_H_ */
