/**
 * Parser module -  reads and interprets the user's inputs. This module supports the following functions:
 *
 * num_of_fixed_cells - the user inputs a number that represents the number of "fixed" cells:
 * 						cells with values that never change throughout the game.
 * read_command - reads, interprets and checks validation of the user's inputs lines that represents commands.
 */
#include "command_stack.h"
#include "ReadingAux.h"
#define MAX_COMMAND_LENGTH 256
#define COMMAND_NAMES {"mark_errors", "guess", "hint", "guess_hint", "autofill", "print_board", "set", "validate", "undo", "redo", \
	"num_solutions", "reset", "exit", "save", "solve", "edit", "generate"}
#define COMMAND_NUMBERS {1, 1, 2, 2, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1, 1, 4, 2}


/**
 * asks the user to input a number that represents the number of "fixed" cells. if the number isn't between 0 to 80,
 * the function prints an error and the process repeats itself until the input is valid.
 * if we reach EOF, the function prints an exit message and terminates.
 *
 * @return
 * the number of "fixed" cells - if the number is valid as described above.
 * -1 - otherwise (EOF)
 */
int num_of_fixed_cells();

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
int read_command(int mode, int args[], char path[], float threshold[]);
