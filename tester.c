///*
// * tester.c
// *
// *  Created on: 12 ׳‘׳“׳¦׳� 2018
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
#include "backtracking_stack.h"
#include "LP_handler.h"
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
void file_system_tester(){
	BOARD a,b;
	load_board("C:/Users/nir/workspace/project/boards/test.txt",&a,&b,3);
	test_print_board(&a,&b);
	save_board("C:/Users/nir/workspace/project/boards/test1.txt",&a,&b,0);
}

void command_stack_tester(){
//	list *s;
//	s = init_list();
//	int args[]= {1,2,3};
//	add_command(s,1,args,0,NULL);
//	add_command(s,2,args,0,NULL);
//	add_command(s,5,NULL,9.6,NULL);
//	print_list(s,1);
//	pop_command(s);
//	pop_command(s);
//	print_list(s,1);
//	forward_current_command(s);
//	forward_current_command(s);
//	add_command(s,3,args,0,NULL);
//	print_list(s,1);
}

void backtracking_stack_tester(){
	stack *s = init_stack(1);
	int x,y,index=80;
	push(s,1,1);
	push(s,2,2);
	//push(s,3,3);
	print_stack(s);
	pop(s,&x,&y,&index);
	printf("pop: x = %d y = %d,index = %d\n",x,y,index);
	pop(s,&x,&y,&index);
	printf("pop: x = %d y = %d,index = %d\n",x,y,index);
	pop(s,&x,&y,&index);
	printf("pop: x = %d y = %d,index = %d\n",x,y,index);
	pop(s,&x,&y,&index);
	printf("pop: x = %d y = %d,index = %d\n",x,y,index);
	if(s->top == NULL){
		printf("nir");
	}
	print_stack(s);

	//delete_stack(s);
	//free(s);
	//print_stack(s);
}

void exhust_backtrack_tester(){
	BOARD a,b;
	load_board("C:/Users/nir/workspace/project/boards/test.txt",&a,&b,3);
	test_print_board(&a,&b);
	printf("res= %d",exhaustive_backtracking(&a));
}

void map_tester(){
	int N = 2;
	int M = 2;
	BOARD *b;
	b = init_board(N,M);
	set_element_to_board(b,1,1,1);
	set_element_to_board(b,0,0,2);
	set_element_to_board(b,2,1,3);
	set_element_to_board(b,1,0,3);
	set_element_to_board(b,0,2,1);

	//set_element_to_board(b,0,2,2);
	print_board(b,b,0,0,0,0);
	int *map = (int*)calloc(N*N*N*M*M*M,sizeof(int));
	int n_var = map_maker(b,map,N*M,N*M*N*M);
	print_array(map,N*N*N*M*M*M);
	//int *array = (int*)calloc(n_var,sizeof(int));
	//row_constraint(b,map,1,2,array,N*M,N*M*N*M);
	//col_constraint(b,map,1,2,array,N*M,N*M*N*M);
	//cell_constraint(b,map,1,0,array,N*M,N*M*N*M);
	//block_constraint(b,map,0,0,2,array,N*M,N*M*N*M);
	//printf("%d\n",n_var);
	//print_array(array,n_var);
	func(b,n_var,map);
}

//int main(){
  //exhust_backtrack_tester();
//	map_tester();
//	return 0;
//}

