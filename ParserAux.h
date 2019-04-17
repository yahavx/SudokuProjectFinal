/*
 * ParserAux.h
 *
 *  Created on: Feb 23, 2019
 *      Author: Orin
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
 */
int isValidNumber(char* stream, int N);

/* get string and cheack if represent a valid number. if true return 1. otherwise 0;
 *
 */
int isNum(char* stream);

/* get a string.
 * if it is a natural number return the numbrt it is, otherwise return -1;
 */
int getNum(char* string);

/*  get num and two more integers represents the beggining of range the end of it
 * return 1 if num is in the range and 0 otherwise;
 *
 */
int isNumInRange(int num, int beginning, int end);

/*
 * function getting buffer that conatains the user imput and
 * return the number of arguments that command as set,edit,solve got from the user;
 */
int numOfArguments(char* stream);

/*
 * check if solve command is legal as entered by the user
 */
int checkSolveCommand(char* stream, char *path);

/*
 * check if edit command is legal as entered by the user
 * and which kind of eddit xomand it is.
 */
CommandType checkEditCommand(char* stream, char* path);

/*
 * check if mark errors command is legal  . retrun 1 if true and 0 otherwise.
 * also assign X parameter in params
 */
int checkMarkErrorsCommand(char* stream, Status mode, int params[3]);

/*
 * Reads from input until the end of the current line.
 */
void finishTheLine();

/* get num of params a command should get and return 1 if the number of
 * params the command got is correct. otherwise print appropriate error and return 0;
 */
int checkParamsNumber(char* stream, int paramsNum);

/*
 * check if bellow detaild commands are legal . retrun 1 if true and 0 otherwise.
 * commands: redo,undo,validate,print board,reset,num_sulutions
 */

int checkSeveralCommands(char* stream, Status mode);

/* if set command is legal and in the right mode and structure, return 1 and update the ints array that concatins
 * the x y z params of set command, of 1 in case of the set command is illegle, in this case
 * the values of the array are not meaningful.
 */
int checkSetCommand(char* stream, int range, Status mode, int params[3]);

/* return 1 if this string can be converted to flout
 * or 0 otherwise
 */
int isFloat(char * stream);

/* return 1 if guess command is legal , which means the commands structure and
 * the board mode is correct, also update threshold argument localy. return 0 otherwise.
 */
int checkGuessCommand(char* stream, Status mode, double* threshold);

/* return 1 if generate command is legal , which means the commands structure and
 * the board mode is correct, also fill in the params array. return 0 otherwise and the values in the array are not informative.
 */
int checkGenerateCommand(char* stream, int range, Status mode, int params[3]);

/* return 1 if Hint or guess hint commands are legal , which means the commands structure and
 * the board mode is correct, also fill in the params array. return 0 otherwise and the values in the array are not informative.
 */
int checkHint_GuessHint_Commands(char* stream, int range, Status mode,
		int params[3]);

/*  return 1 if Autofill command is legal , which means the commands structure and
 * the board mode is correct. return 0 otherwise.
 */
int checkAutofillCommand(char* stream, Status mode);

/*  return 1 if save command is legal , which means the commands structure and
 * the board mode is correct. return 0 otherwise.
 */

int checkSaveCommand(char* stream, Status mode, char path[256]);

/* function recives sorce string stream and destination string path and
 *  copy (safely) the contant in stream into path.
 *  if one of the strings are NULL, do nothing. (will never sent a null to this function)
 *  assumption: path is big enugh to contain stream.
 */
void safeCopy(char* stream, char* path);

#endif /* PARSERAUX_H_ */
