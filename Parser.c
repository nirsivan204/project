#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
int num_of_params(int command) {
	switch (command) {
	case 1: /* set */
		return 3;
	case 2: /* hint */
		return 2;
	default:
		return 0;
	}
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
int search_command(char *string, int mode) {
	int index;
	char *commands[] = {"set", "hint", "validate", "restart", "exit"};
	index = (mode == 1) ? 3 : 0;
	for (; index < 5; index++)
		if (strcmp(string, commands[index]) == 0)
			return ++index;
	return 0;
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
int is_valid_command(char* command_line, int mode) {
	if (strlen(command_line) > MAX_COMMAND_LENGTH) {
		printf("Error: invalid command\n");
	}
	char *string = "";
	int index = 0;
	string = strtok(command_line, " \t\r\n");
	/*
	command[index] = search_command(string, mode);

	if (command[index] == 0) {
		return 0;
	}

	for (index = 1; index <= num_of_params(command[0]); index++) {
		string = strtok(NULL, " \t\r\n");
		if (string == NULL) {
			return 0;
		}
		command[index] = atoi(string);
	}
	*/
	return 1;
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
void read_command(list *s, int mode) {
	char* command_line;
	command_line = (char*)malloc(sizeof(char));
	do{
		printf("Enter next command\n");
		if(fgets(command_line, MAX_COMMAND_LENGTH+2, stdin) == NULL){
			printf("Error: read error occurred\n");
			add_command(s,5,NULL,NULL,0.); /* change 5 to the relevant index of exit. */
			free(command_line);
			return;
		}
	}while(check_if_blank(command_line)==1);
	if (is_valid_command(command_line, mode) == 0) {
		printf("Error: invalid command\n"); /* change: print_relevant_error_message(error) */
		free(command_line);
		read_command(s, mode);
	}
}

/**
 * asks the user to input a number that represents the number of "fixed" cells. if the number isn't between 0 to 80,
 * the function prints an error and the process repeats itself until the input is valid.
 * if we reach EOF, the function prints an exit message and terminates.
 *
 * @return
 * the number of "fixed" cells - if the number is valid as described above.
 * -1 - otherwise (EOF or error)
 */
int num_of_fixed_cells() {
	int num_Of_cells;
	int scanf_res;
	printf("Please enter the number of cells to fill [0-80]:\n");
	scanf_res = scanf("%d", &num_Of_cells);
	if(scanf_res<EOF){
		printf("Error: num_of_fixed_cells() has failed\n");
		return -1;
	}
	if (scanf_res == EOF) {/* EOF */
		printf("Exiting...\n");
		return -1;
	}
	if (is_legal_number(num_Of_cells, 0, 80) == 0) {
		printf("Error: invalid number of cells to fill (should be between 0 and 80)\n");
		return num_of_fixed_cells();
	}
	return num_Of_cells;
}

