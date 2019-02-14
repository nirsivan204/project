///*
// * tester.c
// *
// *  Created on: 12 בדצמ 2018
// *      Author: nir
// */
////#include "MainAux.h"
#include "Solver.h"
#include "Game.h"
#include "command_stack.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "SPBufferset.h"
#include "BoardFileHandler.h"
int current_mode = INIT;

//int this_is_global;
//
//
//void make_random_block(BLOCK rand_block){
//	int i=0;
//	int j=0;
//	for (i=0; i<N; i++){
//		for (j=0; j<M; j++){
//			rand_block[i][j] = 1+(rand()%9);
//		}
//	}
//}
//
//
//void make_random_board(BOARD rand_board){
//	int i=0;
//	int j=0;
//	for (i=0; i<M; i++){
//		for (j=0; j<N; j++){
//			make_random_block(rand_board[i][j]);
//		}
//	}
//}
//
//void test_printing(BOARD rand_board,BOARD fix_board){
//	print_board(rand_board,fix_board);
//}
//
//
//
///*
//int main(int argc, char *argv[]){
//	this_is_global = 1;
//	//BOARD rand_board;
//	BOARD fix_board;
//	BOARD board;
//	BOARD solved_board;
//	SP_BUFF_SET();
//	int seed = atoi(argv[0]);
//	srand(seed);
//	//make_random_board(rand_board);
//	//copy_board(rand_board , fix_board);
//	//test_printing(rand_board,fix_board);
//	//set(rand_board,fix_board,5,4,4);
//	zero_board(board);
//	zero_board(fix_board);
//	zero_board(solved_board);
//
//	//print_board(board,board);
//	initialize_puzzle(35,board,fix_board,solved_board);
//	//print_board(board,board);
//	return 0;
//}
//*/
//
//
//
int main(){
	BOARD a,b;
	list *s;
	load_board("C:/Users/nir/workspace/project/boards/test.txt",&a,&b);
	print_board(&a,&b);
	save_board("C:/Users/nir/workspace/project/boards/test1.txt",&a,&b,0);
	s = init_list();
	int args[]= {1,2,3};
	add_command(s,1,args,"nir",0);
	add_command(s,2,args,NULL,0);
	add_command(s,5,NULL,NULL,9.6);
	print_list(s);
	pop_command(s);
	pop_command(s);
	print_list(s);
	forward_current_command(s);
	forward_current_command(s);
	add_command(s,3,args,"amram",0);
	print_list(s);
	return 0;
}

