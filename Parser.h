/*
 * Parser.h:
 * This module is resposible for receiving input from the user and processing it.
 *
 * Despite checking a command's name is valid, it also checks that it's entered in the correct mode, that the number of parameters supplied is correct, and that every parameter is in the correct range.
 */

#ifndef PARSER_H_
#define PARSER_H_

#include "Game.h"
#include "MainAux.h"

/*
 * This struct is used to pass a command with its parameters from the parser.
 * command is the command type, params are integer parameters (up to 3), and path is for commands that load/save puzzles.
 * threshold is for the guess command.
 */
typedef struct {
	CommandType command;
	int params[3];
	char path[256];/*every command contains 256 chars at most*/
	double threshold;
} Command;


/*
 * Checks if fgets or fgetc has failed. If indeed, prints a message and terminates the program.
 */
void assertFget();

/*
 * @param: fp - file pointer.
 * Checks if fopen has failed. If indeed, prints a message and terminates the program.
 */
int assertFopen(FILE *fp);

/* @params: sudoku - represents game board
 * 		    mode - represents the game status (INIT ,EDIT or SOLVE)
 * Receives and processes input from the user.
 *
 * Returns a Command struct that contains the command type and relevant parameters.
 *
 * If a command contains invalid parameters, wrong number of parameters
 * or current game mode is not suitable for the command, returns illegal command as command type.
 */
Command* parseInput(SudokuBoard* sudoku , Status mode);

#endif /* PARSER_H_ */
