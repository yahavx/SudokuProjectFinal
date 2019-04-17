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

/*@params: File * fp- returned by open()
 * Checks if fopen has failed. If indeed, prints a message and terminates the program.
 */
int assertFopen(FILE *fp);

/* @params - sudokuBoard sudoku- represent game board
 * 		   - mode- represent the game status -INIT ,EDIT  or SOLVE
 * Receives and processes input from the user.
 */
Command* parseInput(SudokuBoard* sudoku , Status mode);

/*Creates a new command struct object which represent ILLEGALY_HANDALED_COMMAND.
 * Return a pointer to it.
 */
Command* createIllegalCommand();

/* Creates a new command struct object, and assign given arguments to it.
 * Return a pointer to the new struct.
 * @pre - path is an initialized array.
 */
Command* createCommand(int params[3], char path[256],CommandType cmd, double threshold);

#endif /* PARSER_H_ */
