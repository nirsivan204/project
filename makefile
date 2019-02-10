CC = gcc
OBJS = main.o Game.o Solver.o BoardHandle.o Parser.o 
EXEC = sudoku
COMP_FLAG = -ansi -Wall -Wextra -Werror -pedantic-errors

$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@
main.o: main.c Game.h Parser.h SPBufferset.h
	$(CC) $(COMP_FLAG) -c $*.c
Game.o: Game.c Game.h Solver.h
	$(CC) $(COMP_FLAG) -c $*.c
Solver.o: Solver.c Solver.h BoardHandle.h
	$(CC) $(COMP_FLAG) -c $*.c
BoardHandle.o: BoardHandle.c BoardHandle.h 
	$(CC) $(COMP_FLAG) -c $*.c
Parser.o: Parser.c Parser.h 
	$(CC) $(COMP_FLAG) -c $*.c
clean:
	rm -f $(OBJS) $(EXEC)
