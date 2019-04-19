#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Parser.h"

/**
 * gets the number of arguments that required by 'command'.
 *
 * @param command - a valid command word
 *
 * @return
 * 3 - if 'command' is "set".
 * 2 - if 'command' is "hint".
 * 0 - otherwise ('command' is "validate", "restart" or "exit").
 */
int get_command_num_of_params(int command) {
	if (command == Mark_errors || command == Guess || command == Save || command == Solve) {
		return 1;
	}
	if (command == Hint || command == Guess_hint || command == Generate) {
		return 2;
	}
	if (command == Set) {
		return 3;
	}
	return 0;
}

/**
 * compare the number of parameters in the user's command to the command's valid number of parameters, calculated with get_command_num_of_params.
 *
 * @param command_name - index of a command.
 * @param num_of_params - number of parameters in the user's command.
 *
 * @return
 * 1 - if user put too many parameters.
 * 0 - if num_of_params is valid.
 * -1 - if user didn't put enough parameters.
 */
int compare_num_of_params(int command_name, int num_of_params, int legal_num_of_params) {
	if (command_name == Edit) { /*edit can contain 0 or 1 parameter*/
		return num_of_params < 2 ? 0 : 1;
	}
	return num_of_params - legal_num_of_params;
}

/**
 * checks if 'number' is between 'minValue' and 'maxValue'.
 *
 * @param number - the checked number.
 * @param minValue - the bottom limit.
 * @param maxValue - the top limit.
 *
 * @return
 * 1 - if 'number' is between 'minValue' and 'maxValue'.
 * 0 - otherwise.
 */
int is_legal_number(int number, int minValue, int maxValue) {
	return number >= minValue && number <= maxValue;
}

/**
 * compare 'string' to each of the valid command words and checks if a match was found.
 * If 'is_puzzle_solved' is 0, then the valid commands are: set, hint, validate, restart and exit.
 * Else ('is_puzzle_solved' is 1), the valid commands are: restart and exit.
 *
 * @param string - a possible command word that was filled in the is_valid_command function.
 * @param is_game_over -  1 if the Sudoku game was solved completely. if so, only restart and exit can be called. 0 otherwise.
 *
 * @return
 * 1 - if the input string can represent a valid command as described above.
 * 0 - otherwise
 */
int get_command_name(char *string) {
	int index;
	char *commands[] = COMMAND_NAMES;
	for (index = 0; index < 17; index++) {
		if (strcmp(string, commands[index]) == 0) {
			return ++index;
		}
	}
	print_invalid_command_error(3, 0, 0, 0, 0);
	return 0;
}

int is_available_in_mode(int command_name, int mode) {
	int from, to;
	assert(is_in_range(mode,1,3));
	switch (mode) {
	case INIT: from = 14, to = 16; break;
	case EDIT: from = 6, to = 17; break;
	case SOLVE: from = 1, to = 16; break;
	}
	if (!is_in_range(command_name, from, to)) {
		print_invalid_command_error(4, mode, command_name, 0, 0);
		return FALSE;
	}
	return TRUE;
}
/**
 * checks if 'line' contains only white spaces.
 *
 * @param line - the user's input line.
 *
 * @return
 * 1 - if 'line' contains only white spaces.
 * 0 - otherwise.
 */
int check_if_blank(char *line){
	unsigned int i;
	for(i=0;i<strlen(line);i++){
		if(!is_white_space(line[i])){
			return 0;
		}
	}
	return 1;
}

void free_params(char **params, int num) {
	int i;
	for(i=0;i<num;i++){
		free(params[i]);
	}
}

int fill_params(char **params, char *string) {
	int num = 0;
	string = strtok(NULL, " \t\r\n");
	while(string && num < 4) {
		params[num] = (char*) malloc((strlen(string)+1) * sizeof(char));
		strcpy(params[num], string);
		num++;
		string = strtok(NULL, " \t\r\n");
	}
	return num;
}

int is_integer(char *string, int i, int length) {
	if (i >= length) {
		return FALSE;
	}
	for (;i<length;i++) {
		if (!isdigit(string[i])){
			return FALSE;
		}
	}
	return TRUE;
}

void get_valid_range(int command_name, int index, int nXm, int range[]) {
	int min, max;
	switch(command_name) {
	case Mark_errors: min = 0, max = 1; break;
	case Generate: min = 0, max = nXm*nXm; break;
	default:
		min = 1, max = nXm;
		if (command_name == Set && index == 2) {
			min = 0;
		}
	}
	range[0] = min, range[1] = max;
}

int update_integer(int command_name, char *string, int args[], int index, int nXm, int numOfEmptyCells) {
	int range[2], input;
	get_valid_range(command_name, index, nXm, range);
	if (!is_integer(string, 0, strlen(string))) {
		print_invalid_command_error(6, range[0], range[1], TRUE, ++index);
		return FALSE;
	}
	input = atoi(string);
	if (!is_in_range(input, range[0], range[1])) {
		print_invalid_command_error(7, range[0], range[1], TRUE, ++index);
		return FALSE;
	}
	if (command_name == Generate && index == 0 && input > numOfEmptyCells) {
		print_invalid_command_error(8, input, 0, TRUE, ++index);
		return FALSE;
	}
	args[index] = input;
	return TRUE;

}

int update_float(char* string, float* threshold) {
	int i = 0;
	while (i<strlen(string)) {
		if (string[i] == '.') {
			break;
		}
		i++;
	}
	if (is_integer(string, 0, i) && is_integer(string, i+1, strlen(string))) {
		*threshold = atof(string);
		if (*threshold >= 0 && *threshold <= 1) {
			return TRUE;
		}
	}
	print_invalid_command_error(6, FALSE, 1, 0, 1);
	return FALSE;
}

int get_invalid_param(int command_name, int num_of_params, char *params[], int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	int index = 0;
	if (num_of_params > 1 || command_name == Mark_errors) { /*command is 'set', 'hint', 'guess_hint', 'generate' or 'mark_errors'*/
		for (; index < num_of_params; index++) {
			if (!update_integer(command_name, params[index], args, index, nXm, numOfEmptyCells)) {
				return index;
			}
		}
	}
	else if (num_of_params == 1) {
		if (command_name == Guess) {
			return !update_float(params[index], threshold) ? 0 : -1;
		}
		strcpy(path, params[index]); /* command is 'save', 'edit' or 'solve'*/
	}
	else if (command_name == Edit) { /* command is 'edit' with no parameters*/
		strcpy(path, "");
	}
	*threshold = 0; /*for Gurobi purposes: if command isn't 'guess', we will use 0 as threshold*/
	return -1;
}

/**
 * stores the line's first string (the command word) in 'command' as its first element, and then calls search_command.
 * If search_command returns -1, then the function returns -1 ('command_input' represents an invalid command).
 * Otherwise, it calls num_of_params and checks if 'command_input' contains enough arguments. If not, it returns -1
 * ('command_input' represents an invalid command), and otherwise it stores the arguments in their proper place in 'command'.
 *
 * @param command_input - an array containing the input line that was filled in the read_command function.
 * @param command - the array that represents a command:
 * the first element represents the command word and the rest of the elements represents the command parameters (if there are any).
 * @param is_game_over -  1 if the Sudoku game was solved completely. if so, only restart and exit can be called. 0 otherwise.
 *
 * @return
 * 1 - if the input line can represent a valid command as described above.
 * 0 - otherwise
 */
int get_command(char* command_line, int mode, int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	char *string = "", **params = (char**)calloc(4,sizeof(char *));
	int command_name, num_of_params, legal_num_of_params, compare, invalid_param;
	if (check_if_blank(command_line)==1) {
		free(params);
		return FALSE;
	}
	if (strlen(command_line) > MAX_COMMAND_LENGTH) {
		print_invalid_command_error(2, 0, 0, 0, 0);
		free(params);
		return FALSE;
	}
	string = strtok(command_line, " \t\r\n");
	command_name = get_command_name(string);
	if (command_name == 0 || !is_available_in_mode(command_name, mode)) {
		free(params);
		return FALSE;
	}
	num_of_params = fill_params(params, string);
	legal_num_of_params = get_command_num_of_params(command_name);
	compare = compare_num_of_params(command_name, num_of_params, legal_num_of_params);
	if (compare != 0) {
		print_invalid_command_error(5, compare, command_name, legal_num_of_params, 0);
		free_params(params, num_of_params);
		free(params);
		return FALSE;
	}
	invalid_param = get_invalid_param(command_name, num_of_params, params, args, path, threshold, nXm, numOfEmptyCells);
	free_params(params, num_of_params);
	free(params);
	return invalid_param > -1 ? FALSE : command_name;
}


/**
 * reads the user's input line and creates an array that will contain it.
 * If the input line isn't null, it calls is_valid_command (depends also on the 'is_puzzle_solved' parameter). If is_valid_command
 * returns -1, the function prints an error and starts over. If the input line is null, the 'command' parameter stores 5 ("exit")
 * as its first element (which will lead to termination of the program).
 *
 * @param command - the array that represents a command:
 * the first element represents the command word and the rest of the elements represents the command parameters (if there are any).
 * @param is_game_over - 1 if the Sudoku game was solved completely. if so, only restart and exit can be called. 0 otherwise.
 */
int read_command(int mode, int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	int command;
	char* command_line = NULL;
	printf("Please enter your next command:\n");
	fflush(stdout);
	command_line = (char*)malloc((MAX_COMMAND_LENGTH+2)*sizeof(char));
	if(fgets(command_line, MAX_COMMAND_LENGTH+2, stdin) == NULL){
		print_invalid_command_error(1, 0, 0, 0, 0);
		free(command_line);
		return Exit;
	}
	command = get_command(command_line, mode, args, path, threshold, nXm, numOfEmptyCells);
	free(command_line);
	return command;
}

