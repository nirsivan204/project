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

	BOARD game_board, fix_board;
	int N, M, mode, markErrors, args[3], command, execute, isValidBoard, isUpdatedBoard;
	char path[MAX_COMMAND_LENGTH];
	float threshold;
	list command_list;
	mode = INIT, markErrors = TRUE, isValidBoard = FALSE, isUpdatedBoard = FALSE;
	initialize_puzzle(&game_board, &fix_board, &command_list);
	do {
		command = read_command(mode, args, path, &threshold, N, M);
		printf("result of read_command is %d. arg0 == %d, arg1 == %d, arg2 == %d, threshold == %f, path== %s\n", \
				command, args[0], args[1], args[2], threshold, path);
		if (command > 0) { /* command was successfully read */
			execute = execute_command(command, &game_board, &fix_board, &command_list, &markErrors, &mode, \
				&isValidBoard, &isUpdatedBoard, &N, &M, args, path, threshold);
			printf("result of execute_command is %d. markErrors = %d, mode = %d, isValidBoard = %d, isUpdatedBoard = %d, N = %d, M = %d\n", \
					execute, markErrors, mode, isValidBoard, isUpdatedBoard, N, M);
		}
	} while (execute > -1);

//	BOARD a,b;
//	load_board("C:/Users/ebay/eclipse-workspace/project/boards/test.txt",&game_board,&fix_board, mode);
//	print_board(&game_board,&fix_board,markErrors,mode,&isValidBoard, &isUpdatedBoard);
//	load_board("boards/test.txt",&a,&b,3);
//	print_board(&a,&b,markErrors,mode,&isValidBoard, &isUpdatedBoard);
//	printf("%d\n", get_element_from_board(&a,0,1));
//	set_element_to_board(&a, 0,3,0);
//	printf("%d\n", is_valid_insertion(&a, 0,1,1));
//	printf("%d\n", is_valid_insertion(&a, 0,1,2));
//	BOARD a,b;
//	list *s;
//	s = init_list();
//	print_list(s,1);
//	int args[]= {1,2,3};
//	add_command(s,1,args,0,NULL);
//	add_command(s,2,args,0,NULL);
//	add_command(s,5,NULL,9.6,NULL);
//	print_list(s,1);
//	pop_command(s);
//	pop_command(s);
//	add_command(s,4,args,0,NULL);
//	print_list(s,1);
//	pop_command(s);
//	forward_current_command(s);
//	print_list(s,1);
//	add_command(s,3,args,0,NULL);
//	print_list(s,1);
	return 0;
}
