/**
 * Parser module -  reads and interprets the user's inputs. This module supports the following functions:
 *
 * num_of_fixed_cells - the user inputs a number that represents the number of "fixed" cells:
 * 						cells with values that never change throughout the game.
 * read_command - reads, interprets and checks validation of the user's inputs lines that represents commands.
 */
#include "Errors.h"
#define MAX_COMMAND_LENGTH 256
#define COMMAND_NAMES {"mark_errors", "guess", "hint", "guess_hint", "autofill", "print_board", "set", "validate", "undo", "redo", \
	"num_solutions", "reset", "save", "solve", "edit", "exit", "generate"}

/**
 * reads the user's input line and creates an array that will contain it.
 * If the input line isn't null, it checks if the input is valid (depends also on the 'is_puzzle_solved' parameter).
 * If invalid, the function prints an error and starts over. Otherwise (if valid) the 'command' parameter stores the command word
 * and its arguments. If the input line is null, the 'command' parameter stores 5 ("exit") as its first element (which will lead to
 * termination of the program).
 *
 * @param command - the array that represents a command:
 * the first element represents the command word and the rest of the elements represents the command arguments (if there are any).
 * @param is_puzzle_solved - 1 if the sudoku game is solved completely, 0 else.
 */
int read_command(int mode, int args[], char path[], float* threshold, int N, int M);
