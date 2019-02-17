/**
 * Game module -  contains all possible moves, stores and manipulates the game status. This module supports the following function:
 */
#ifndef GAME_H_
#define GAME_H_
#include "Solver.h"
#include "command_stack.h"

enum Mode {INIT = 1,EDIT = 2,SOLVE = 3};
enum Command {Mark_errors = 1, Guess = 2, Hint = 3, Guess_hint = 4, Autofill = 5, Print_board = 6, Set = 7, Validate = 8, \
	Undo = 9, Redo = 10, Num_solutions = 11, Reset = 12, Exit = 13, Save = 14, Solve = 15, Edit = 16, Generate = 17};

/*
 *  execute_command - performs a move on the current puzzle, according to a specific given command.
 */
int execute_command(int command, BOARD *board, BOARD *fix_board, BOARD *solved_board, list *command_list, int *markErrors, \
		int *mode, int args[], char path[], float* threshold);

#endif
