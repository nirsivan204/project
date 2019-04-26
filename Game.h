/**
 * Game module -  contains all possible moves, stores and manipulates the game status. This module supports the following functions:
 *
 * initialize_puzzle    - prints an announcement of the beginning of the game.
 * execute_command 		- performs a move on the current puzzle, according to a specific given command.
 */
#ifndef GAME_H_
#define GAME_H_
#include "backtracking_stack.h"
#include "LP_handler.h"
#include "BoardFileHandler.h"
#include "CommandStack.h"

/*
 *  this function prints an announcement of the beginning of the game.
 */
void initialize_puzzle();

/*
 *  this function performs a move on the current puzzle, according to a specific given command.
 *
 *  @param command			- a number that represents one of the 17 legal command names.
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param command_list		- a pointer to a list of the user's moves.
 *  @param markErrors		- a pointer to a number that determines whether errors in the puzzle are displayed.
 *  @param mode				- a pointer to a number that represents one of the 3 playable modes.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *  @param nXm				- a pointer to a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a pointer to a number that represents the current number of empty cells in the puzzle.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a float that is used as a parameter for one of the commands.
 *
 *  @return 1 if the command was executed successfully, 0 if failed, and -1 if the program needs to be terminated.
 */
int execute_command(int command, BOARD **board, BOARD **marking_board, list **command_list, int *markErrors, int *mode, \
		int* isValidBoard, int* isUpdatedBoard, int* nXm, int* numOfEmptyCells, int *args, char *path, float threshold);

#endif
