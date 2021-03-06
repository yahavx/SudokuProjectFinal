CC = gcc
OBJS = main.o MainAux.o Game.o Solver.o Parser.o GameAux.o LinkedMovesList.o Stack.o ParserAux.o LPSolver.o FileHandle.o StandardLinkedList.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

all: $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $(EXEC)
	
$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c MainAux.h Game.h Solver.h Parser.h FileHandle.h
	$(CC) $(COMP_FLAG) -c $*.c	
Game.o: Game.c MainAux.h Solver.h Game.h LinkedMovesList.h ParserAux.h GameAux.h FileHandle.h
	$(CC) $(COMP_FLAG) -c $*.c
MainAux.o: MainAux.c Game.h GameAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c MainAux.h Game.h Stack.h GameAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c MainAux.h Game.h Parser.h ParserAux.h LinkedMovesList.h
	$(CC) $(COMP_FLAG) -c $*.c
GameAux.o: GameAux.c Game.h Solver.h LPSolver.h
	$(CC) $(COMP_FLAG) -c $*.c
LinkedMovesList.o: LinkedMovesList.c LinkedMovesList.h MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
Stack.o: Stack.c Stack.h MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
ParserAux.o: ParserAux.c Game.h MainAux.h Parser.h Solver.h LinkedMovesList.h ParserAux.h
	$(CC) $(COMP_FLAG) -c $*.c
FileHandle.o: FileHandle.c MainAux.h ParserAux.h GameAux.h LPSolver.h
	$(CC) $(COMP_FLAG) -c $*.c
StandardLinkedList.o: StandardLinkedList.c StandardLinkedList.h MainAux.h
	$(CC) $(COMP_FLAG) -c $*.c
LPSolver.o: LPSolver.c Game.h Solver.h GameAux.h MainAux.h StandardLinkedList.h
	$(CC) $(COMP_FLAG) $(GUROBI_COMP) -c $*.c

clean:
	rm -f $(OBJS) $(EXEC)

