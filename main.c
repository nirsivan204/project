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

	char* command_line;
	command_line = (char*)malloc(sizeof(char));
	if(fgets(command_line, 2+MAX_COMMAND_LENGTH , stdin) == NULL){
		printf("null");
		free(command_line);
		return 0;
	}
//	float f;
//	scanf("%d",&f);
	if (strlen(command_line) > MAX_COMMAND_LENGTH) {
		printf("error: length = %d", (int)strlen(command_line));
		free(command_line);
		return 0;
	}
	printf("%s, %d", command_line, (int)strlen(command_line));
	free(command_line);
	return 0;
}
