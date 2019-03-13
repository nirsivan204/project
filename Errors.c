
#include "Errors.h"

void print_system_error(int error_number) {
	printf("System Error: ");
	switch (error_number) {

	}
	printf("\n");
}

void print_mode_error(int mode, int command_name) {
	char* string;
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

void print_num_of_params_error(int compare, int command_name, int legal_num_of_params) {
	printf(compare > 0 ? "Too many parameters. " : "Not enough parameters. ");
	printf("This command's number of parameters required is exactly ");
	printf(command_name == Edit ? "0 or 1." : "%d.", legal_num_of_params);
}

void print_invalid_parameter_error(int error_number, int isInt, int param_num, int min, int max) {
	char* commandOrBoard = error_number == 6 ? "is illegal for this command" : "has an illegal value for this current board state";
	char* type = isInt ? "an integer" : "a float";
	printf("Parameter %d %s. ", param_num, commandOrBoard);
	printf("This parameter should be %s with minimum value of %d and maximum value of %d.", type, min, max);
}

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

void print_invalid_file_error(int error_number) {
	printf("Invalid File Error: ");
	switch (error_number) {
	case 1: printf("Problem opening file."); break;
	case 2: printf("Problem writing in file."); break;
	case 3: printf("File doesn't contain a legal puzzle format."); break;
	}
	printf("\n");
}

void print_no_moves_error(int command_name) {
	printf("No moves to %sdo.", command_name == Undo ? "un" : "re");
}

void print_invalid_move_error(int error_number, int arg1, int arg2) {
	printf("Invalid Move Error: ");
	switch (error_number) {
	case 1: printf("Board is erroneous. This command can be executed only on a valid board, make sure there are no erroneous cells."); break;
	case 2: printf("Board is unsolvable. This command can be executed only on a solvable board."); break;
	case 3: printf("Cell <%d,%d> is fixed. This command cannot be executed on a fixed cell.", arg1, arg2); break;
	case 4: printf("Cell <%d,%d> already contains a value. This command can be executed only on an empty cell.", arg1, arg2); break;
	case 5: print_no_moves_error(arg1);
	}
	printf("\n");
}
