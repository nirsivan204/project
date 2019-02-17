#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "assert.h"
#include "Parser.h"
#include "ReadingAux.h"

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
	if (command == 1 || command == 2 || command == 14 || command == 15) {
		return 1;
	}
	if (command == 3 || command == 4 || command == 17) {
		return 2;
	}
	if (command == 7) {
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
int compare_num_of_params(int command_name, int num_of_params) {
	if (command_name == 16) { /*edit can contain 0 or 1 parameter*/
		return num_of_params < 2 ? 0 : 1;
	}
	return num_of_params - get_command_num_of_params(command_name);
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
int get_command_name(char *string, int mode) {
	int index, top_index;
	char *commands[] = COMMAND_NAMES;
	assert(mode == 1 || mode == 2 || mode == 3);
	switch (mode) {
	case 1: index = 14, top_index = 15; break; /* Init mode */
	case 2: index = 5, top_index = 16; break; /* Edit mode */
	case 3: index = 0, top_index = 15; break; /* Solve mode */
	}
	for (; index <= top_index; index++) {
		if (strcmp(string, commands[index]) == 0) {
			return ++index;
		}
	}
	return 0;
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

void free_params(char *params[], int num) {
	unsigned int i;
	for(i=0;i<num;i++){
		free(params[i]);
	}
}

int fill_params(char *params[], char *string) {
	int num = 0;
	string = strtok(NULL, " \t\r\n");
	while(string && num < 4) {
		params[num] = (char*) malloc((int)strlen(string) * sizeof(char));
		strcpy(params[num], string);
		num++;
		string = strtok(NULL, " \t\r\n");
	}
	return num;
}

int is_integer(char *string, int i) {
	for (;i<strlen(string);i++) {
		if (!isdigit(string[i])){
			return 0;
		}
	}
}
int update_integer(char *string, int args[], int index) {
	for (int i=0;i<strlen(string);i++) {
		if (!isdigit(string[i])){
			return 0;
		}
	}
	args[index] = atoi(string);
	return 1;
}

int update_float(char* string, float* threshold) {
	int i, num_of_dots = FALSE;
	while (i<strlen(string) && num_of_dots == FALSE) {
		if (string[i] == '.') {
			num_of_dots = TRUE;
		}
		else if (string[i] != '0') {
			return FALSE;
		}
	}
	for (;i<strlen(string);i++) {
		if (!isdigit(string[i])){
			if (string[i] != '.' || num_of_dots == TRUE) {
				return 0;
			}
			num_of_dots = TRUE;
		}
	}
	*threshold = atof(string);
	return TRUE;
//	int num_of_dots = FALSE;
//	for (int i=0;i<strlen(string);i++) {
//		if (!isdigit(string[i])){
//			if (string[i] != '.' || num_of_dots == TRUE) {
//				return 0;
//			}
//			num_of_dots = TRUE;
//		}
//	}
//	*threshold = atof(string);
//	return 1;
}

int get_invalid_param(int command_name, int num_of_params, char *params[], int args[], char path[], float* threshold) {
	int index = 0;
	if (num_of_params > 1 || command_name == 1) { /* num_of_params is 2 or 3, or command is 'mark_errors'*/
		for (; index < num_of_params; index++) {
			if (!update_integer(params[index], args, index)) {
				return index;
			}
		}
	}
	else if (num_of_params > 0) { /* num_of_params == 1 */
		if (command_name == 2) { /* command is 'guess'*/
			if (!update_float(params[index], threshold)) {
				return index;
			}
		}
		else { /* command is 'save', 'edit' or 'solve'*/
			strcpy(path, params[index]);
		}
	}
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
int get_command(char* command_line, int mode, int args[], char path[], float* threshold) {
	char *string = "", *params[4];
	int command_name, num_of_params, compare, invalid_param;
	if (check_if_blank(command_line)==1) {
		return 0;
	}
	if (strlen(command_line) > MAX_COMMAND_LENGTH) {
		printf("Error: too many characters in a single line\n");/* change: print_relevant_error_message(error) */
		return 0;
	}
	string = strtok(command_line, " \t\r\n");
	command_name = get_command_name(string, mode);
	if (command_name == 0) {
		printf("Error: incorrect name\n");/* change: print_relevant_error_message(error) */
		return 0;
	}
	num_of_params = fill_params(params, string);
	compare = compare_num_of_params(command_name, num_of_params);
	if (compare != 0) {
		printf("%s", compare > 0 ? "Error: too many parameters\n" : "Error: not enough parameters\n");/* change: print_relevant_error_message(error) */
		free_params(params, num_of_params);
		return 0;
	}
	invalid_param = get_invalid_param(command_name, num_of_params, params, args, path, threshold);
	if (invalid_param > -1) {
		printf("Error: invalid parameter: %d\n", ++invalid_param);/* change: print_relevant_error_message(error) */
		free_params(params, num_of_params);
		return 0;
	}
	free_params(params, num_of_params);

	return command_name;
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
int read_command(int mode, int args[], char path[], float* threshold) {
	int command;
	char* command_line;
	printf("Enter next command\n");
	fflush(stdout);
	command_line = (char*)malloc(sizeof(char));
	if(fgets(command_line, MAX_COMMAND_LENGTH+2, stdin) == NULL){
		printf("Error: read error occurred\n");
		free(command_line);
		return 0;
	}
	command = get_command(command_line, mode, args, path, threshold);
	free(command_line);
	return command;
}

