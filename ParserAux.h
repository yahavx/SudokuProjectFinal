/*
 * ParserAux.h:
 * This module is resposible for receiving input from the user and proccesing it.
 *
 * It also validates the commands are entered in the corrent mode, and that command's parameters are in the correct range.
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

/*@params - stream- string to chack
 *  N- integer
 * Returns 1 iff the current token is a number between 0 to N, includes.
 *  @pre : stream !=NULL;
 */
int isValidNumber(char* stream, int N);

/*@params- stream- string to check
 *  This function gets string and check if represent a valid number. if true return 1. otherwise 0;
 * @pre : stream !=NULL;
 */
int isNum(char* stream);

/* @params- stream- string to check
 * This function gets a string.
 * if it is a natural number return the numbrt it is, otherwise return -1;
 */
int getNum(char* string);

/*@params- num- integer to check
 *		 - begginging- int represent lowwer bound of range
 *		 - end- integer reprsent upper bound of range
 * This function gets integer num and two more integers represents the beggining of range the end of it
 * return 1 if num is in the range and 0 otherwise;
 */
int isNumInRange(int num, int beginning, int end);

/*@params - stream- string to chack
 * This function getting buffer that conatains the user input and
 * return the number of arguments that  a command as set,edit,solve, hint, guess. guess_hint etc got from the user;
 */
int numOfArguments(char* stream);

/*@params - stream- string to chack
 * 		  - path- chars array to assign into
 * This function check if solve command is legal as entered by the user,
 * return 1 and fill path if true, return 0 otherwise and the contant of path is meaningless and should be ignored.
 */
int checkSolveCommand(char* stream, char *path);

/*
 * This function check if edit command is valid according to the project instructions.
 * Also, this function create distinction between edit without file name and edit with file name.
 * In case of edit with file name , the function update path to contain the filename. otherwise path's content is meaningless.
 * The function return 1 iff the command structut is valid  according to the project instructions.
 */
CommandType checkEditCommand(char* stream, char* path);

/*@params - stream- string to chack
 * 		  - path- chars array to assign into
 *This function check if mark errors command is valid as entered by the user, according to the project instructions.
 *Retrun 1 if true and 0 otherwise.
 * In case the command is valid, the gfunction assign X parameter  of the command to the int params array, in params[0] cell.
 */
int checkMarkErrorsCommand(char* stream, Status mode, int params[3]);

/*
 * Reads from input until the end of the current line.
 */
void finishTheLine();

/* @params - stream- string to chack
 * 		  - paramsNum - integer represents the number of parameters command should get.
 * This function gets num of params a command should get in paramsNum argument and return 1 if the number of
 * params the command got is correct. otherwise print appropriate error and return 0;
 */
int checkParamsNumber(char* stream, int paramsNum);

/*@params - stream- stream which is the user's input to check.
 * 		  - mode- represent the current game status (INIT, SOLVE or EDIT).
 * This function check if the bellow detaild commands are legal according to the project instructions. retrun 1 if true and 0 otherwise.
 * commands: redo,undo,validate,print board,reset,num_sulutions
 */
int checkSeveralCommands(char* stream, Status mode);

/* @params: stream- represent user input that recognized as set commamnd and its arguments.
 * 			mode- the game status (INIT, EDIT or SOLVE).
 * 			params- int array , which will conatin the X Y Z params in cells 0 1 2 of the array,in correspondence.
 * This function check if set command is legal and in the right mode of game and  ha right command structure according to the project
 * instructions.
 * If true, the function update the int params array that concatins
 * the X Y Z params of set command, and return 1.
 * Otherwise return 0. In this case , the set command as entered by the user is illegle and
 * the values params thet the params array contains are meaningless and should be ignored.
 */
int checkSetCommand(char* stream, int range, Status mode, int params[3]);

/* @parans - stream which represent string to check.
 * This function return 1 if this string can be converted to flout
 * or 0 otherwise
 * @pre: stream!=NULL
 */
int isFloat(char * stream);

/* @params - stream - stream of input entered by the user
 * 		   - mode- the game status (INIT, EDIT, SOLVE)
 * 		   - threshold - pointer to int variable which the X  parameter of guess X commsnd will be assigne into.
 *This function return 1 if guess command is valid accorging to the priject instructions , which means the commands structure and
 * the board mode is correct, in this case the function also update the threshold variable localy.
 * Otherwise,the function return 0 , and threshold variable is meaningless and shpuld be ignored.
 */
int checkGuessCommand(char* stream, Status mode, double* threshold);

/* @params - stream - stream of input entered by the user
 * 		   - range- integer represent the max value of cells in row(or column) in the sudokuboard.
 * 		   - mode- the game status (INIT, EDIT, SOLVE)
 * 		   - params- int array , which will conatin the X Y  params in cells 0 1  of the array,in correspondence
 * This function return 1 if generate command is valid according to the project instructions , which means the commands structure and
 * the game mode is correct, in tthis case italso fill in the params array with X Y params of generate command as entered by the user.
 * Otherwise, return 0  and the values in the array are meaningless and should be ignored.
 */
int checkGenerateCommand(char* stream, int range, Status mode, int params[3]);

/* @params - stream - stream of input entered by the user
 * 		   - range- integer represent the max value of cells in row(or column) in the sudokuboard.
 * 		   - mode- the game status (INIT, EDIT, SOLVE)
 * 		   - params- int array , which will conatin the X Y  params in cells 0 1  of the array,in correspondence.
 * This function return 1 if Hint or guess hint commands are valid according to the project instructions , which means the commands structure and
 * the board mode is correct, also fill in the params array. return 0 otherwise and the values in the array are not informative.
 */
int checkHint_GuessHint_Commands(char* stream, int range, Status mode,
		int params[3]);

/* @params - stream - stream of input entered by the user
 * 		   - mode- the game status (INIT, EDIT, SOLVE)
 * This function return 1 if Autofill command is valid according to the project instructions,
 * which means the commands structure and the game mode are correct. Otherwise return 0;
 */
int checkAutofillCommand(char* stream, Status mode);

/* @params - stream - stream of input entered by the user
 * 		   - mode- the game status (INIT, EDIT, SOLVE)
 * 		   - path- chars array , which will conatin the file name parameter of the command save [fileName].
 * This function return 1 if save command is valid according to the project instructions , which means the commands structure and
 * the board mode is correct. Othewise return 0 .
 */
int checkSaveCommand(char* stream, Status mode, char path[256]);

/* @params - stream - sorce string
 * 		   - path - destination string
 *  @ pre : length (path)>= length (stream)
 *  This function recives sorce string stream and destination string path and
 *  copy (safely) the contant in stream into path, char by char.
 *  If one of the strings are NULL, do nothing. (will never sent a null poiner to this function)
 */
void safeCopy(char* stream, char* path);

/*
 * Initialize the array by setting all elements to zero.
 * @param c - pointer to the array of char, N - array size
 */
void initializeArray(char c[], int N);

#endif /* PARSERAUX_H_ */
