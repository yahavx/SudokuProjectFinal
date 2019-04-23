/*
 * ParserAux.h:
 * This module is resposible for receiving input from the user and proccesing it.
 *
 * It also validates the commands are entered in the corrent mode, and that command's parameters are in the correct range.
 *
 * All validation functions checks that a command is executed and the correct mode, has the correct number of parameters,
 * and each is in the corret range. Returns 1 if valid, 0 if not, and prints errors accordingly.
 */

#ifndef PARSERAUX_H_
#define PARSERAUX_H_
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include "MainAux.h"
#include "Parser.h"
#include "Solver.h"
#include "LinkedMovesList.h"
#include "ParserAux.h"

/*
 * Returns 1 iff the current token is a number between 0 to N, includes.
 * @pre: stream != NULL.
 */
int isValidNumber(char* stream, int N);

/*
 * Gets a string and checks if it represents an integer. If true return 1, otherwise 0.
 * @pre: stream != NULL.
 */
int isNum(char* stream);

/*
 *
 * Gets a string and checks if it represents a float. If true return 1, otherwise 0.
 * @pre: stream != NULL
 */
int isFloat(char * stream);

/*
 * Converts a string to an integer and returns it.
 * If the string is not an integer, returns -1.
 */
int getNum(char* string);

/*
 * Checks if num is the range [beggining,end]. Returns 1 iff yes.
 */
int isNumInRange(int num, int beginning, int end);

/*
 * Initialize the array by setting all elements to zero.
 * @params: c - pointer to the array
 *          N - array size
 */
void initializeArray(char c[], int N);

/*
 * Copies a string, char by char.
 * @params: src - source string
 * 		    dst - destination string
 * If one of them is NULL, does nothing.
 * @pre : length(dst) >= length(src)
 */
void safeCopy(char* src, char* dst);

/*
 * Reads from input until the end of the current line.
 */
void finishTheLine();

/*
 * Returns 1 if stream contains paramsNum tokens.
 * Used to check if a command has the corrent number of parameters.
 * If not, prints appropriate error (too many or not enough parameters).
 */
int validateParamsNumber(char* stream, int paramsNum);

/*
 * Checks if stream contains no parameters. Returns 1 if yes, otherwise 0 and prints appropriate error.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          command - the checked command
 * @pre: command is available in Edit and Solve only (redo, undo, validate, print_board, reset or num_solutions).
 */
int validateZeroParameters(char* stream, Status mode, CommandType command);


/*
 * Validates solve command. Returns 1 iff command is valid.
 * @params: path - a pointer to assign the parameter to.
 *          stream - the 2nd token of the command entered by the user.
 * If @ret == 0, path should be ignored.
 */
int validateSolve(char* stream, char *path);

/*
 * Validtes edit cocmmand.
 * Returns EDIT_WITHOUT_FILE_NAME if no parameter supplied, and EDIT_WITH_FILE_NAME i 1 parameter is supplied.
 * @params: path - a pointer to assign the parameter to.
 *          stream - the 2nd token of the command entered by the user.
 * If validation failed, returns ILLEGALY_HANDLED_COMMAND.
 */
CommandType validateEdit(char* stream, char* path);

/*
 * Validates mark errors command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          params - array to fill with the command parameter (to first slot).
 * If @ret == 0, params should be ignored.
 */
int validateMarkErrors(char* stream, Status mode, int params[3]);


/*
 * Validates set command. Returns 1 iff command is valid.
  * @params: mode - the current mode of the game.
 *           stream - the 2nd token of the command entered by the user.
 *           params - array to fill with the command parameters (in order of appearance).
 *           range - max legal value for a parameter (i.e. game board dimesnion).
 * If @ret == 0, params should be ignored.
 */
int validateSet(char* stream, int range, Status mode, int params[3]);



/*
 * Validates guess command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          threshold - pointer to store the parameter in.
 * If @ret == 0, threshold should be ignored.
 */
int validateGuess(char* stream, Status mode, double* threshold);

/*
 * Validates generate command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          threshold - pointer to store the parameter in.
 *          range - max legal value for a parameter.
 * If @ret == 0, params should be ignored.
 */
int validateGenerate(char* stream, int range, Status mode, int params[3]);

/*
 * Validates guess command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          threshold - pointer to store the parameter in.
 *          range - max legal value for a parameter.
 * If @ret == 0, params should be ignored.
 */
int validateHintAndGuessHint(char* stream, int range, Status mode,
		int params[3], CommandType command);

/*
 * Validates autofill command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 */
int validateAutofill(char* stream, Status mode);

/*
  * Validates save command. Returns 1 iff command is valid.
 * @params: mode - the current mode of the game.
 *          stream - the 2nd token of the command entered by the user.
 *          threshold - pointer to store the parameter in.
 * If @ret == 0, dst should be ignored.
 */
int validateSave(char* src, Status mode, char path[256]);


#endif /* PARSERAUX_H_ */
