CC = gcc
OBJS = main.o MainAux.o Game.o Solver.o Parser.o GameAux.o LinkedMovesList.o Stack.o ParserAux.o test.o LPSolver.o FileHandle.o StandardLinkedList.o
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c MainAux.h Game.h Solver.h Parser.h test.h SPBufferset.h FileHandle.h
	$(CC) $(COMP_FLAG) -c $*.c	
Game.o: Game.c MainAux.h Solver.h Game.h SPBufferset.h LinkedMovesList.h ParserAux.h GameAux.h test.h FileHandle.h
	$(CC) $(COMP_FLAG) -c $*.c
MainAux.o: MainAux.c Game.h GameAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c MainAux.h Game.h Stack.h GameAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c MainAux.h Game.h Parser.h ParserAux.h LinkedMovesList.h
	$(CC) $(COMP_FLAG) -c $*.c
GameAux.o: GameAux.c Game.h Solver.h
	$(CC) $(COMP_FLAG) -c $*.c
LinkedMovesList.o: LinkedMovesList.c LinkedMovesList.h MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
ParserAux.o: ParserAux.c Game.h MainAux.h Parser.h Solver.h LinkedMovesList.h ParserAux.h
	$(CC) $(COMP_FLAG) -c $*.c
test.o: test.c Game.h MainAux.h Parser.h Solver.h SPBufferset.h Stack.h GameAux.h ParserAux.h LPSolver.h FileHandle.h StandardLinkedList.h
	$(CC) $(COMP_FLAG) -c $*.c
FileHandle.o: FileHandle.c MainAux.h ParserAux.h GameAux.h LPSolver.h
	$(CC) $(COMP_FLAG) -c $*.c
StandardLinkedList.o: StandardLinkedList.c StandardLinkedList.h MainAux.h

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
LPSolver.o: LPSolver.c Game.h Solver.h GameAux.h MainAux.h StandardLinkedList.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)

