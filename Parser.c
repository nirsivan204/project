
#include "Parser.h"

/*
 *  this function returns the number of parameters required for a given command
 *  @param command	- a number that represents one of the 17 legal command names.
 *
 *  @return the number of parameters required for the command.
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

/*
 * this function compares the number of parameters in the user's command to the command's valid number of parameters, calculated with get_command_num_of_params.
 *
 * @param command_name 			 - a number that represents one of the 17 legal command names..
 * @param num_of_params 		 - the number of parameters in the user's command.
 * @param legal_num_of_params	 - the number of parameters required for the command.
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

/*
 * this function compares a given string to each of the legal command words and checks if a match was found.
 * @param string - a possible command word.
 *
 * @return a number (between 1 to 17) that represents a legal command if 'string' is equal to the command's string representation as described in the header file, 0 if no match was found.
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

/*
 * this function checks if a given command's execution is available in a given mode.
 * @param command_name	- a number that represents one of the 17 legal command names.
 * @param mode			- a number that represents one of the 3 playable modes.
 *
 * @return 1 if command_name is in the relevant range of valid numbers as described below, 0 otherwise.
 */
int is_available_in_mode(int command_name, int mode) {
	int from, to;
	switch (mode) {
	case INIT: from = 14, to = 16; break; /*Init mode allows only commands represented by a number between 14 to 16*/
	case EDIT: from = 6, to = 17; break; /*Edit mode allows only commands represented by a number between 6 to 17*/
	case SOLVE: from = 1, to = 16; break; /*Solve mode allows only commands represented by a number between 1 to 16*/
	}
	if (!is_in_range(command_name, from, to)) {
		print_invalid_command_error(4, mode, command_name, 0, 0);
		return FALSE;
	}
	return TRUE;
}

/*
 * this function checks if a given string contains only white spaces.
 * @param line - the user's input line.
 *
 * @return 1 if 'line' contains only white spaces, 0 otherwise.
 */
int check_if_blank(char *line){
	unsigned int i;
	for(i=0;i<strlen(line);i++){
		if(!is_white_space(line[i])){
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * this function frees the memory allocation of elements of a given array.
 * @param params - an array of strings representing potential parameters for a command.
 * @param num 	 - the number of elements freed.
 */
void free_params(char **params, int num) {
	int i;
	for(i=0;i<num;i++){
		free(params[i]);
	}
}

/*
 * this function fills a given array of strings with parameters read from a given string line.
 * @param params - an array of strings representing potential parameters for a command.
 * @param string - the user's input (starting from the first parameter after the command name).
 *
 * @return the number of parameters given by the user (0-3).
 */
int fill_params(char **params, char *string) {
	int num = 0;
	string = strtok(NULL, " \t\r\n");
	while(string && num < 4) {
		params[num] = (char*) malloc((strlen(string)+1) * sizeof(char));
		if (params[num] == NULL) {
			print_system_error(1, "couldn't malloc a buffer for one of the parameters");
		}
		strcpy(params[num], string);
		num++;
		string = strtok(NULL, " \t\r\n");
	}
	return num;
}

/*
 * this function checks if a sub-string of a given string can represent a non-negative integer.
 * @param string - a string representing a parameter of a command, given by the user.
 * @param i 	 - the left boundary of the sub-string checked.
 * @param length - the right boundary of the sub-string checked.
 *
 * @return 1 if the sub-string contains only digits, 0 otherwise.
 */
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

/*
 * this function fills a given array with the minimum and the maximum legal values of a given parameter index, according to a given command.
 *  @param command_name	- a number that represents one of the 17 legal command names.
 *  @param index		- a number that represents a parameter's index (0-first, 1-second, 2-third).
 *  @param nXm			- a number that represents the number of cells in each row, column and block.
 *  @param range		- an empty array of integers that is going to be filled with two elements.
 */
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

/*
 *  this function converts a parameter from string to integer and puts it in the arguments array.
 *  @param command_name		- a number that represents one of the 17 legal command names.
 *  @param string 			- a strings that represents a potential parameter for the command.
 *  @param args				- an array that will contain all integer parameters.
 *  @param index			- a number that represents a parameter's index (0-first, 1-second, 2-third).
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *
 *  @return 1 if the conversion is successful and the parameter's value is legal for the command, 0 otherwise.
 */
int update_integer(int command_name, char *string, int args[], int index, int nXm, int numOfEmptyCells) {
	int range[2], input;
	get_valid_range(command_name, index, nXm, range);
	if (!is_integer(string, 0, strlen(string))) { /* parameter isn't a non-negative integer */
		print_invalid_command_error(6, range[0], range[1], TRUE, ++index);
		return FALSE;
	}
	input = atoi(string);
	if (!is_in_range(input, range[0], range[1])) { /* parameter's value is out of legal range */
		print_invalid_command_error(7, range[0], range[1], TRUE, ++index);
		return FALSE;
	}
	if (command_name == Generate && index == 0 && input > numOfEmptyCells) { /* parameter's value is out of legal range (special case) */
		print_invalid_command_error(8, input, 0, TRUE, ++index);
		return FALSE;
	}
	args[index] = input;
	return TRUE;

}

/*
 *  this function converts a parameter from string to float and sets it as a value for a pointer.
 *  @param string 			- a strings that represents a potential parameter for the command.
 *  @param threshold		- a pointer to the converted parameter.
 *
 *  @return 1 if the parameter is a float between 0 to 1, 0 otherwise.
 */
int update_float(char* string, float* threshold) {
	int i = 0;
	if (strcmp(string, "0") == 0 || strcmp(string, "1") == 0) {
		*threshold = atof(string);
		return TRUE;
	}
	while (i<strlen(string)) {
		if (string[i] == '.') {
			break;
		}
		i++;
	}
	if (is_integer(string, 0, i) && is_integer(string, i+1, strlen(string))) { /* parameter is a non-negative float */
		*threshold = atof(string);
		if (*threshold >= 0 && *threshold <= 1) { /* parameter's value is in legal range */
			return TRUE;
		}
	}
	print_invalid_command_error(6, FALSE, 1, 0, 1);
	return FALSE;
}

/*
 *  for each parameter in a given array, this function checks if it's a valid parameter according to a given command, and returns its index if not.
 *  @param command_name		- a number that represents one of the 17 legal command names.
 *  @param num_of_params	- the number of parameters checked.
 *  @param params 			- an array of strings representing potential parameters for a command.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a pointer to a float that is used as a parameter for one of the commands.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *
 *  @return the minimal index of an invalid parameter (0-2), or -1 if all parameters are valid.
 */
int get_invalid_param(int command_name, int num_of_params, char *params[], int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	int index = 0;
	if (num_of_params > 1 || command_name == Mark_errors) { /*command requires integers ('set', 'hint', 'guess_hint', 'generate' or 'mark_errors')*/
		for (; index < num_of_params; index++) {
			if (!update_integer(command_name, params[index], args, index, nXm, numOfEmptyCells)) {
				return index;
			}
		}
	}
	else if (num_of_params == 1) {
		if (command_name == Guess) { /* command requires a float */
			return !update_float(params[index], threshold) ? 0 : -1;
		}
		/* else- command requires a string ('save', 'edit' or 'solve') */
		strcpy(path, params[index]); /* path is now a path of a file (not necessary valid). */
	}
	else if (command_name == Edit) { /* command is 'edit' with no parameters */
		strcpy(path, "");
	}
	*threshold = 0; /*for Gurobi purposes: if command isn't 'guess', we will use 0 as threshold*/
	return -1;
}

/*
 * this function validates the user's input line (using various validation methods) and gets the command that needs to be executed.
 *
 *  @param command_line		- a string given by the user as an input.
 *  @param mode				- a number that represents one of the 3 playable modes.
 *  @param args				- an array of integers which are used as parameters for some of the commands.
 *  @param path				- a string that is used as a parameter for some of the commands.
 *  @param threshold		- a pointer to a float that is used as a parameter for one of the commands.
 *  @param nXm				- a number that represents the number of cells in each row, column and block.
 *  @param numOfEmptyCells	- a number that represents the current number of empty cells in the puzzle.
 *
 *  @return an integer that represents one of the 17 legal command names, or 0 if one of the validation methods fails.
 */
int get_command(char* command_line, int mode, int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	int command_name, num_of_params, legal_num_of_params, compare, invalid_param;
	char *string = NULL;
	char **params = NULL;
	params = (char**)calloc(4,sizeof(char *));
	if (params == NULL) {
		print_system_error(1, "couldn't malloc a buffer for the command");
	}
	string = "";
	if (check_if_blank(command_line)==1) { /* empty input line */
		free(params);
		return FALSE;
	}
	if (strlen(command_line) > MAX_COMMAND_LENGTH) {  /* too many characters */
		print_invalid_command_error(2, 0, 0, 0, 0);
		free(params);
		return FALSE;
	}
	string = strtok(command_line, " \t\r\n"); /* the first argument found in the user's input line */
	command_name = get_command_name(string);
	if (command_name == 0 || !is_available_in_mode(command_name, mode)) { /* illegal command */
		free(params);
		return FALSE;
	}
	num_of_params = fill_params(params, string);
	legal_num_of_params = get_command_num_of_params(command_name);
	compare = compare_num_of_params(command_name, num_of_params, legal_num_of_params);
	if (compare != 0) { /* illegal number of parameters */
		print_invalid_command_error(5, compare, command_name, legal_num_of_params, 0);
		free_params(params, num_of_params);
		free(params);
		return FALSE;
	}
	invalid_param = get_invalid_param(command_name, num_of_params, params, args, path, threshold, nXm, numOfEmptyCells);
	free_params(params, num_of_params);
	free(params);
	return invalid_param > -1 ? FALSE : command_name; /* return command_name if all parameters are valid */
}


/*
 * this function reads the user's input line and creates an array that will contain it.
 * If the input line isn't null, it calls the 'get_command' method.
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
int read_command(int mode, int args[], char path[], float* threshold, int nXm, int numOfEmptyCells) {
	int command;
	char* command_line = NULL;
	printf("Please enter your next command:\n");
	fflush(stdout);
	command_line = (char*)malloc((MAX_COMMAND_LENGTH+2)*sizeof(char));
	if (command_line == NULL) {
		print_system_error(1, "couldn't malloc a buffer for the command");
	}
	if(fgets(command_line, MAX_COMMAND_LENGTH+2, stdin) == NULL){
		print_invalid_command_error(1, 0, 0, 0, 0);
		free(command_line);
		return Exit;
	}
	command = get_command(command_line, mode, args, path, threshold, nXm, numOfEmptyCells);
	free(command_line);
	return command;
}

