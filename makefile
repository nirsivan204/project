CC = gcc
OBJS =  Game.o BoardFileHandler.o BoardHandle.o Parser.o Errors.o AuxModule.o LPHandler.o command_stack.o BacktrackingStack.o LargeArray.o
EXEC = sudoku-console
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56


$(EXEC): main.o $(OBJS)
	$(CC) main.o $(OBJS) $(GUROBI_LIB) -o $@ -lm
tester: tester.o $(OBJS)
	$(CC) tester.o $(OBJS) $(GUROBI_LIB) -o $@ -lm
main.o: main.c Game.h Parser.h SPBufferset.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
tester.o: tester.c Game.h Parser.h SPBufferset.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Game.o: Game.c Game.h Solver.h BoardFileHandler.h command_stack.h LPHandler.h BacktrackingStack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
BoardFileHandler.o: BoardFileHandler.c BoardFileHandler.h Errors.h BoardHandle.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
BoardHandle.o: BoardHandle.c BoardHandle.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h 
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Errors.o: Errors.c Errors.h AuxModule.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
AuxModule.o: AuxModule.c AuxModule.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
LPHandler.o: LPHandler.c LPHandler.h LargeArray.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
command_stack.o: command_stack.c command_stack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
BacktrackingStack.o: BacktrackingStack.c BacktrackingStack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
LargeArray.o: LargeArray.c LargeArray.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
clean:
	rm -f main.o tester.o $(OBJS) $(EXEC)
