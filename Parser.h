/*
 * Parser.h
 *
 *  Created on: 21 áôáø× 2019
 *      Author: yahav
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Game.h"
#include "MainAux.h"

typedef struct {
	CommandType command;
	int params[3];
	char path[256];/*every command contains 256 chars at most*/
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


Command* createCommand(int params[3], char path[256],CommandType cmd, double threshold);

#endif /* PARSER_H_ */
