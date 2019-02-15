/*
 * main.c
 * amram
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */

#include "SPBufferset.h"
//#include "Game.h"
#include "Parser.h"
#include <time.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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


//	float f = 0.;
//	scanf("%d",&f);
	int args[3];
	char path[MAX_COMMAND_LENGTH] = "amram nevo";
	float threshold[1];
	printf("%d\n", read_command(3, args, path, threshold));
//	printf("%d, %d, %s, %f", args[0], args[1], path, threshold[0]);
	return 0;
}
