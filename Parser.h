/**
 * Parser module -  reads and interprets the user's inputs. This module supports the following function:
 *
 * read_command - reads, interprets and checks validation of the user's inputs lines that represents commands.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Errors.h"
#define MAX_COMMAND_LENGTH 256
#define COMMAND_NAMES {"mark_errors", "guess", "hint", "guess_hint", "autofill", "print_board", "set", "validate", "undo", "redo", \
	"num_solutions", "reset", "save", "solve", "edit", "exit", "generate"}

/*
 * this function reads the user's input line and creates an array that will contain it.
 * If the input line isn't null, it calls various methods in order to interpret the command and its parameters while validating them.
 *
 *  @param mode				- a number that represents one of the 3 playable modes.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a pointer to a float that is used as a parameter for one of the commands.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *
 *  @return an integer that represents one of the 17 legal command names, or 0 if 'get_command' fails.
 */
int read_command(int mode, int args[], char path[], float* threshold, int nXm, int numOfEmptyCells);
