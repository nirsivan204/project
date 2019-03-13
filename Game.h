/**
 * Game module -  contains all possible moves, stores and manipulates the game status. This module supports the following function:
 */
#ifndef GAME_H_
#define GAME_H_
#include "Solver.h"
#include "BoardFileHandler.h"
#include "command_stack.h"

void initialize_puzzle(BOARD *board, BOARD *fix_board, list *command_list);

/*
 *  execute_command - performs a move on the current puzzle, according to a specific given command.
 */
int execute_command(int command, BOARD *board, BOARD *fix_board, list *command_list, int *markErrors, \
		int *mode, int* isValidBoard, int* isUpdatedBoard, int* N, int* M, int args[], char path[], float threshold);

#endif
