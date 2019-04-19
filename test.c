/*
 * test.c
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
#include "Solver.h"
#include "SPBufferset.h"
#include "Stack.h"
#include "GameAux.h"
#include "ParserAux.h"
#include "string.h"
#include <ctype.h>
#include "Gurobi.h"
#include "FileHandle.h"
#include "StandardLinkedList.h"

void stackTest() {
	Stack* s = initialize();
	int i, j, lastUsed;
	printf("Empty: %d\n", isEmpty(s));
	emptyStack(s);
	push(5, 2, 1, s);
	printf("Empty: %d\n", isEmpty(s));

	pop(s);
	printf("Empty: %d\n", isEmpty(s));

	push(2, 3, 4, s);
	printf("(%d,%d)\n", top(s).i, top(s).j);
	printf("(%d,%d)\n", pop(s).i, top(s).j);
	printf("Empty: %d\n", isEmpty(s));

	push(4, 5, 6, s);
	push(9, 2, 9, s);
	printf("Empty: %d\n", isEmpty(s));
	push(2, 4, 1, s);

	popAndUpdate(s, &i, &j, &lastUsed);
	printf("(%d,%d),%d\n", i, j, lastUsed);

	emptyStack(s);
	printf("Empty: %d\n", isEmpty(s));

	destroyStack(s);
}

void gameTest() {
	int i, j, z, mark = 1;
	double threshold;
	List *list;
	int loaded;
	Status mode;
	SudokuBoard *sudoku, *sudoku2;
	char command[20];
	list = createNewList();
	printf("Enter board dimensions:\nn = ");
	scanf("%d", &i);
	printf("m = ");
	scanf("%d", &j);

	sudoku = initializeBoard(i, j);
	sudoku2 = sudoku;
	mode = SOLVE;

	while (1) {
		printf("Enter command: ");
		scanf("%s", command);

		if (strcmp(command, "solve") == 0) { /* command 1 */
			scanf("%s", command);
			sudoku2 = load(sudoku, command, &loaded);
			if (sudoku2 != sudoku) {
				sudoku = sudoku2;
				mode = SOLVE;
			}
			continue;
		}

		if (strcmp(command, "edit") == 0) { /* command 2 */
			scanf("%s", command);
			if (strcmp(command, "null") == 0) {
				sudoku = load(sudoku, NULL, &loaded);
				continue;
			}

			sudoku2 = load(sudoku, command, &loaded);
			if (sudoku2 != sudoku) {
				sudoku = sudoku2;
				mode = EDIT;
			}
			continue;
		}

		if (strcmp(command, "mark_errors") == 0) { /* command 3 */
			scanf("%d", &i);
			mark = i;
			continue;
		}

		if (strcmp(command, "print_board") == 0 || strcmp(command, "p") == 0) { /* command 4 */
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "set") == 0) { /* command 5 */
			scanf("%d %d %d", &i, &j, &z);
			set(sudoku, i, j, z, mode, list);
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "validate") == 0) {/* command 6 - not avilable in eclipse */
			i = validate(sudoku);
			if (i != -1) {
				if (i == 1) {
					printf("Board is solveable.\n");
				} else if (i == 0) {
					printf("Board is unsolveable.\n");
				} else
					printf("Error.\n");
			}
			continue;
		}

		if (strcmp(command, "guess") == 0) { /* command 7 - not avilable in eclipse */
			scanf("%lf", &threshold);
			guess(sudoku, threshold, list);
			continue;
		}

		if (strcmp(command, "generate") == 0) { /* command 8 - not avilable in eclipse */
			scanf("%d %d", &i, &j);
			generate(sudoku, i, j, list);
			continue;
		}

		if (strcmp(command, "undo") == 0) { /* command 9 */
			undo(sudoku, list);
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "redo") == 0) { /* command 10 */
			redo(sudoku, list);
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "save") == 0) { /* command 11 */
			scanf("%s", command);
			save(sudoku, command, mode);
			continue;
		}

		if (strcmp(command, "hint") == 0) { /* command 12 - not avilable in eclipse */
			scanf("%d %d", &i, &j);
			hint(sudoku, i, j);
			continue;
		}

		if (strcmp(command, "guess_hint") == 0) { /* command 13 - not avilable in eclipse */
			scanf("%d %d", &i, &j);
			guessHint(sudoku, i, j);
			continue;
		}

		if (strcmp(command, "num_solutions") == 0) { /* command 14 */
			findNumberOfSolutions(sudoku);
			continue;
		}

		if (strcmp(command, "autofill") == 0) { /* command 15 */
			autofill(sudoku, list);
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "reset") == 0) { /* command 16 */
			reset(sudoku, list);
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "exit") == 0) {/* command 17 */
			break;
		}

		/* for testing */

		if (strcmp(command, "list") == 0) {
			printList2(list);
			continue;
		}

		if (strcmp(command, "fix") == 0) {
			scanf("%d %d", &i, &j);
			getCell(sudoku, i, j)->fixed = 1;
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "unfix") == 0) {
			scanf("%d %d", &i, &j);
			getCell(sudoku, i, j)->fixed = 0;
			printBoard(sudoku, mode, mark);
			continue;
		}

		if (strcmp(command, "mode") == 0) {
			scanf("%s", command);
			if (strcmp(command, "solve") == 0)
				mode = SOLVE;
			else if (strcmp(command, "edit") == 0)
				mode = EDIT;
			else if (strcmp(command, "?") == 0) {
				if (mode == EDIT)
					printf("Mode: edit.\n");
				else
					printf("Mode: solve.\n");
			} else
				printf("Invalid mode.\n");
			continue;
		}

		if (strcmp(command, "solution") == 0) {
			getSolution(sudoku);
			continue;
		}

		if (strcmp(command, "issolved") == 0) {
			if (isSolved(sudoku) && mode == SOLVE) {
				printf("Board is solved.\n");
			} else
				printf("Board is not solved.\n");
			continue;
		}

		if (strcmp(command, "rl") == 0) {
			resetList(list);
			continue;
		}

		if (strcmp(command, "help") == 0) {
			printf(
					"Available commands:\n1. solve <file>\n2. edit <file>\n3. mark_errors 0/1\n4. print_board (or p)\n");
			printf(
					"5. set <i> <j> <value>\n9. undo\n10. redo\n11. save <file>\n14. num_solutions\n15. autofill\n16. reset\n17. exit\n\n");
			printf(
					"On nova also:\n6. validate\n7. guess <threshold>\n8. generate <X> <Y>\n12. hint <X> <Y>\n13. guess_hint <X> <Y>\n\n");
			printf(
					"For testing: list, fix, unfix, mode, solution, issolved, resetlist.\n");
			continue;
		}

		printf("Invalid command.\n");
	}
	destroyBoard(sudoku);
	destroyList(list);
	printf("Exiting...\n");
}

void stackBacktrackTest() {

	SudokuBoard *sudoku = initializeBoard(2, 3);
	getCell(sudoku, 0, 0)->fixed = 1;
	getCell(sudoku, 0, 0)->value = 2;
	getCell(sudoku, 5, 5)->fixed = 1;
	getCell(sudoku, 5, 5)->value = 2;
	printBoard3(sudoku);

	sudoku = initializeBoard(2, 3);
	getCell(sudoku, 0, 0)->fixed = 1;
	getCell(sudoku, 0, 0)->value = 2;
	getCell(sudoku, 5, 5)->fixed = 1;
	getCell(sudoku, 5, 5)->value = 2;

	printBoard3(sudoku);

}

void parserTest() {
	char s[] = "as as as as as as";
	int i, x = strlen(s);
	char *t1, *t2;
	printf("string: %s\n", s);
	t1 = strtok(s, " \n\r\t");
	t2 = strtok(s, " \n\r\t");
	while (t1 != NULL) {
		printf("\nstrtok...\n");
		printf("t1: %s, t2: %s, original: %s\n", t1, t2, s);

		for (i = 0; i < x; i++) {

			printf("%d ", s[i]);
		}
		t1 = strtok(NULL, " \n\r\t");
	}
}
void parserTest2() {
	Status mode = INIT;
	SudokuBoard *sudoku = initializeBoard(3, 3);
	Command *c = parseInput(sudoku, mode);
	printf(
			"Command: %d, param 1: %d, param2: %d, param3: %d\npath: %s, threshold: %f",
			c->command, c->params[0], c->params[1], c->params[2], c->path,
			c->threshold);
}

void saveLoadTest() {
	int loaded;
	SudokuBoard *sudoku = initializeBoard(3, 3);
	sudoku = load(sudoku, "input.txt", &loaded);
	printBoard3(sudoku); /* print loaded file */
	getCell(sudoku, 2, 2)->value = 1;
	getCell(sudoku, 2, 2)->fixed = 1;
	printBoard3(sudoku); /* print loaded file with a change */
	save(sudoku, "output.txt", SOLVE);
	sudoku = load(sudoku, "output.txt", &loaded);
	printBoard3(sudoku); /* save change, load, and print again */
}

void fileTest() {

	FILE *input = fopen("input.txt", "r");
	char str[5];
	fgets(str, 5, input);
	printf("%s", str);
	fgets(str, 5, input);
	printf("%s", str);
	fgets(str, 5, input);
	printf("%s", str);

}

typedef struct yosi {
	int x;
	int y;
} Y;

Y* createY() {
	Y *bla = malloc(17);
	bla->y = 1337;
	return bla;
}

void getY(Y *bla) {
	printf("Y: %d", bla->y);
}

int bla() {

	Y *bla = malloc(17);
	printf("%p", (void*) bla);
	exit(0);
	return 0;
}

void slistTest() {
	StandardList *l = createNewStandardList();
	printStandardList(l);
	addNewStandardMove(l, 5, 10);
	printStandardList(l);

	printf("5 is found: %d\n", lookupInStandardList(l, 5));
	printf("4 is found: %d\n", lookupInStandardList(l, 4));

	addNewStandardMove(l, 4, 20);
	printStandardList(l);

	addNewStandardMove(l, 6, 30);
	printStandardList(l);

	printf("4 is found: %d\n", lookupInStandardList(l, 4));

	destroyStandardList(l);
	printf("head val after destroy: %d", l->Head->val);
	exit(0);
}

void LPSolTest() {
	LPSolution *boardSol = initializeLPSolution(2);
	StandardList **mapping = getLists(boardSol);
	int i, j, N = getN(boardSol);

	addVariable(boardSol,1,1,5,20);
	addVariable(boardSol,1,1,6,30);

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (mapping[i * N + j] != NULL) {
				printf("list (%d,%d):", i, j);
				printStandardList(mapping[i * N + j]);
			}
		}
	}


	printf("(0,0,3): %d\n", getVariableIndex(boardSol, 0, 0, 3));
	printf("(1,1,5): %d\n", getVariableIndex(boardSol, 1, 1, 5));
	printf("(1,1,2): %d\n", getVariableIndex(boardSol, 1, 1, 2));
	printf("(1,1,6): %d\n", getVariableIndex(boardSol, 1, 1, 6));

	destroyLPSolution(boardSol);

	exit(0);
}
int test() {
	SudokuBoard *sudoku = initializeBoard(3, 3);
	destroyBoard(sudoku);
	srand(time(NULL));
	LPSolTest();
	gameTest();
	exit(0);
}

