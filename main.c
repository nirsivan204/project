/*
 * main.c
 * amram
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */

#include "SPBufferset.h"
#include "Game.h"
#include "Parser.h"
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(int argc, char *argv[]){
	BOARD game_board, fix_board;
	int nXm, mode, markErrors, args[3], command, execute, isValidBoard, isUpdatedBoard, numOfEmptyCells;
	char path[MAX_COMMAND_LENGTH];
	float threshold;
	list command_list;
	mode = INIT, markErrors = TRUE, isValidBoard = FALSE, isUpdatedBoard = FALSE;
	initialize_puzzle(&game_board, &fix_board, &command_list);
	do {
		command = read_command(mode, args, path, &threshold, nXm, numOfEmptyCells);
		printf("result of read_command is %d. arg0 == %d, arg1 == %d, arg2 == %d, threshold == %f, path== %s\n", \
				command, args[0], args[1], args[2], threshold, path);
		if (command > 0) { /* command was successfully read */
			execute = execute_command(command, &game_board, &fix_board, &command_list, &markErrors, &mode, \
				&isValidBoard, &isUpdatedBoard, &nXm, &numOfEmptyCells, args, path, threshold);
			printf("result of execute_command is %d. markErrors = %d, mode = %d, isValidBoard = %d, isUpdatedBoard = %d, numOfEmptyCells = %d, nXm = %d\n", \
					execute, markErrors, mode, isValidBoard, isUpdatedBoard, numOfEmptyCells, nXm);
		}
	} while (execute > -1);

	return 0;
}

