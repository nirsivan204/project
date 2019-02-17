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


//	float f = 0.;
//	scanf("%d",&f);

	int args[3];
	char path[MAX_COMMAND_LENGTH] = "amram nevo";
	float threshold[1];
	printf("result of read_command is %d.\n", read_command(3, args, path, threshold));
//	printf("%d, %d, %s, %f", args[0], args[1], path, threshold[0]);
//	printf("%d", 1);

//	int num = 0;
//	int i = 0, j = 0;
//	char *s = "a  b c   d e";
//	char c = s[j];
//	while (c != '\0' && num < 4) {
//		if (is_white_space(c)) {
//			num += i<j ? 1 : 0;
//			i = j+1;
//		}
//		j++;
//		c = s[j];
//	}
//	num += i<j ? 1 : 0;
//	int n =0;
//	char *s = "12345";
//	for (int i=0;i<strlen(s);i++) {
//		if (!isdigit(s[i])){
//			if (s[i] != '.' || n > 0) {
//				printf("false\n");
//				return 0;
//			}
//			n++;
//		}
//	}
//
//	printf("num = %f\n", atof(s));
	return 0;
}
