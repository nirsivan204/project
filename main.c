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
//	BOARD *game_board;
//	BOARD *fix_board;
//	BOARD *solved_board;
//	int fix;
//	int is_game_over = 0;
//	int command[4];
//	int seed = atoi(argv[argc-1]);
//	srand(seed);
//	SP_BUFF_SET();
//	do {
//		fix = num_of_fixed_cells();
//		if(fix < 0){/*problem at reading the input*/
//			break;
//		}
//		initialize_puzzle(fix, game_board, fix_board, solved_board);/*initialize all the boards*/
//		do{
//			read_command(command, is_game_over);/*read next command*/
//			is_game_over = execute_command(command, game_board, fix_board, solved_board);/*execute command*/
//		}while(command[0] < 4);/*set, hint or validate*/
//	}while(command[0] == RESTART);

//	BOARD *game_board;
//	BOARD *fix_board;
//	BOARD *solved_board;
	int mode = 3, markErrors = 1, args[3], command, execute;
	char path[MAX_COMMAND_LENGTH];
	float threshold = 0;
	list *command_list;
	command = read_command(mode, args, path, &threshold);
	printf("result of read_command is %d. threshold == %f\n", command, threshold);
//	execute = execute_command(command, game_board, fix_board, solved_board, command_list, &markErrors, &mode, args, path, &threshold);

//	printf("%d, %d,%d,%d,%d\n", (int) 0, (int) 0.3, (int) 0.7, (int) 1, (int) 1.5);
//	printf("%d", 1);
//
//	printf("num = %f\n", atof(s));
	return 0;
}
