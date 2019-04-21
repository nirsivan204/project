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
	BOARD *game_board = NULL, *fix_board = NULL;
	int nXm, mode, markErrors, args[3], command = 0, execute = 0, isValidBoard, isUpdatedBoard, numOfEmptyCells;
	char path[MAX_COMMAND_LENGTH];
	float threshold;
	list *command_list = NULL;
	mode = INIT, markErrors = TRUE, isValidBoard = FALSE, isUpdatedBoard = FALSE;
	initialize_puzzle();
	do {
		command = read_command(mode, args, path, &threshold, nXm, numOfEmptyCells);
		if (command > 0) { /* command was successfully read */
			execute = execute_command(command, &game_board, &fix_board, &command_list, &markErrors, &mode, \
				&isValidBoard, &isUpdatedBoard, &nXm, &numOfEmptyCells, args, path, threshold);
		}
	} while (execute > -1);
	delete_board(game_board);
	delete_board(fix_board);
	delete_list(command_list);
	return 0;
}

