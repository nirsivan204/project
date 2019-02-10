/**
 * Game module -  contains all possible moves, stores and manipulates the game status. This module supports the following function:
 */
#include "Solver.h"
enum Command {SET = 1, HINT = 2, VALIDATE = 3, RESTART = 4, EXIT = 5};

/*
 *  execute_command - performs a move on the current puzzle, according to a specific given command.
 */
int execute_command(int *command, BOARD board, BOARD fix_board, BOARD solved_board);
