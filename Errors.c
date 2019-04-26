
#include "Errors.h"

/*
 * this function prints a relevant system error message.
 * @param error_number 	- a number that represents a specific type of errors.
 * @param function_name - a string that describes a specific problem.
 */
void print_system_error(int error_number, char* function_name) {
	printf("System Error: ");
	switch (error_number) {
	case 1: printf("Problem with memory allocation in function %s.",function_name); exit(errno);
	case 2: printf("Problem in Gurobi."); exit(errno);
	}
	printf("\n");
}

/*
 * this function prints an error that was caused by using an unavailable command in a specific mode.
 * @param mode 			- a number that represents one of the 3 playable modes.
 * @param command_name	- a number that represents one of the 17 legal command names.
 */
void print_mode_error(int mode, int command_name) {
	char* string = NULL;
	switch (mode) {
	case INIT: string = "Init mode"; break;
	case EDIT: string = "Edit mode"; break;
	case SOLVE: string = "Solve mode"; break;
	}
	printf("You are now in %s. ", string);
	if (command_name > 16) {
		string = "Edit mode";
	}
	else if (command_name < 6) {
		string = "Solve mode";
	}
	else {
		string = "Edit mode and Solve mode";
	}
	printf("This command is available only in %s.", string);
}

/*
 * this function prints an error that was caused by an invalid number of parameters for a specific command.
 * @param compare 				- a non-zero number that determines whether too many parameters were given by the user, or too few.
 * @param command_name			- a number that represents one of the 17 legal command names.
 * @param legal_num_of_params	- a number that represents the exact number of parameters required.
 */
void print_num_of_params_error(int compare, int command_name, int legal_num_of_params) {
	printf(compare > 0 ? "Too many parameters. " : "Not enough parameters. ");
	printf("This command's number of parameters required is exactly ");
	printf(command_name == Edit ? "0 or 1." : "%d.", legal_num_of_params);
}

/*
 * this function prints an error that was caused by an invalid parameter of a command.
 * @param error_number 	- a number (6,7 or 8) that represents a specific type of errors.
 * @param arg1 			- the minimum legal value of the parameter (if error_number is 6 or 7),
 *                        or the number of empty cells given in the 'generate' command (if error_number is 8).
 * @param arg2 			- the maximum legal value of the parameter.
 * @param isInt 		- a number that determines whether the required parameter should be an integer (TRUE) or a float (FALSE).
 * @param param_num 	- the index of the parameter.
 */
void print_invalid_parameter_error(int error_number, int arg1, int arg2, int isInt, int param_num) {
	char* commandOrBoard = error_number == 6 ? "is illegal for this command" : "has an illegal value for this current board state";
	char* type = isInt ? "an integer" : "a float";
	printf("Parameter %d %s. ", param_num, commandOrBoard);
	if (error_number == 8) {
		printf("There are less than %d empty cells.", arg1);
		return;
	}
	printf("This parameter should be %s with minimum value of %d and maximum value of %d.", type, arg1, arg2);
}

/*
 * this function prints an error that was caused by an invalid input given by the user.
 * @param error_number 	- a number that represents a specific type of errors.
 * @param arg1 			- a parameter used for the error message.
 * @param arg2 			- a parameter used for the error message.
 * @param arg3 			- a parameter used for the error message.
 * @param arg4 			- a parameter used for the error message.
 */
void print_invalid_command_error(int error_number, int arg1, int arg2, int arg3, int arg4) {
	printf("Invalid Command Error: ");
	switch (error_number) {
	case 1: printf("Read error occurred."); break;
	case 2: printf("Too many characters in a single line. Make sure your commands don't contain more than 256 characters."); break;
	case 3: printf("Illegal command name."); break;
	case 4: print_mode_error(arg1, arg2); break;
	case 5: print_num_of_params_error(arg1, arg2, arg3); break;
	default: print_invalid_parameter_error(error_number, arg1, arg2, arg3, arg4); break;
	}
	printf("\n");
}

/*
 * this function prints an error that was caused by problems in file handling.
 * @param error_number 	- a number that represents a specific type of errors.
 */
void print_invalid_file_error(int error_number) {
	printf("Invalid File Error: ");
	switch (error_number) {
	case 1: printf("Problem opening file."); break;
	case 2: printf("Problem writing in file."); break;
	case 3: printf("File doesn't contain a legal puzzle format."); break;
	}
	printf("\n");
}

/*
 * this function prints an error that was caused by using a command that requires a valid/solvable board, while the current board isn't.
 * @param isErroneous	- a number that determines whether the board is erroneous (TRUE) or unsolvable (FALSE).
 * @param isGenerate	- a number that determines whether the given command is 'generate' (TRUE) or not (FALSE).
 */
void print_invalid_board_error(int isErroneous, int isGenerate) {
	printf("Board is %s. ", isErroneous ? "erroneous" : "unsolvable");
	if (isGenerate) {
		printf("Puzzle generation failed with this board.");
		return;
	}
	printf("This command can be executed only on a %s.", isErroneous ? "valid board, make sure there are no erroneous cells" : "solvable board");
}

/*
 * this function prints an error that was caused by using 'undo'/'redo' when there aren't any moves on the list to undo/redo, respectively.
 * @param command_name	- a number that represents the 'undo' command or the 'redo' command.
 */
void print_no_moves_error(int command_name) {
	printf("No moves to %sdo.", command_name == Undo ? "un" : "re");
}

/*
 * this function prints an error that was caused by invalid moves.
 * @param error_number 	- a number that represents a specific type of errors.
 * @param arg1 			- a parameter used for the error message.
 * @param arg2 			- a parameter used for the error message.
 * @param arg3 			- a parameter used for the error message.
 */
void print_invalid_move_error(int error_number, int arg1, int arg2, int arg3) {
	int isGenerate = arg3 == Generate;
	if (!isGenerate) {
		printf("Invalid Move Error: ");
	}
	switch (error_number) {
	case 1: print_invalid_board_error(TRUE, isGenerate); break;
	case 2: print_invalid_board_error(FALSE, isGenerate); break;
	case 3: printf("Cell <%d,%d> is fixed. This command cannot be executed on a fixed cell.", arg1, arg2); break;
	case 4: printf("Cell <%d,%d> already contains a value. This command can be executed only on an empty cell.", arg1, arg2); break;
	case 5: print_no_moves_error(arg1);
	}
	printf("\n");
}
