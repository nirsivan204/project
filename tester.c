///*
// * tester.c
// *
// *  Created on: 12 ׳‘׳“׳¦׳� 2018
// *      Author: nir
// */
////#include "MainAux.h"
#include "Solver.h"
#include "Game.h"
#include "Parser.h"
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
	load_board("/specific/a/home/cc/students/csguests/nirsivan/Cproject/project/boards/test.txt",&a,&b,3);
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
	//func(b,n_var,map);
}

void put_sol_test(){
	int N = 2;
	int M = 2;
	BOARD *b;
	b = init_board(N,M);
	set_element_to_board(b,1,1,1);
	set_element_to_board(b,0,0,2);
	set_element_to_board(b,2,1,3);
	set_element_to_board(b,1,0,3);
	set_element_to_board(b,0,2,1);
	int *map = (int*)calloc(N*N*N*M*M*M,sizeof(int));
	map_maker(b,map,N*M,N*M*N*M);
	double sol[] = {1,0,0,1,1,1,0,1,0,0,1,0,1,0,1,0,0,1,0,1,0,1,0,0,0};
	put_sol_in_board(b,map,sol,N*M,N*M*N*M,0);
	print_board(b,b,0,0,0,0);
}

void girobi_test(){

	BOARD a,b;

	load_board("/specific/a/home/cc/students/csguests/nirsivan/Cproject/project/boards/test2.txt",&a,&b,3);
	print_board(&a,&b,0,0,0,0);
	int N = a.N;
	int M = a.M;
	printf("N=%d,M=%d",N,M);
	int *map = (int*)calloc(N*N*N*M*M*M,sizeof(int));
	int num_of_vars = map_maker(&a,map,N*M,N*M*N*M);
	double *sol = (double *)calloc(num_of_vars,sizeof(double));
	//srand(time(0));
	gurobi(&a,num_of_vars,map,0,sol);
    put_sol_in_board(&a,map,sol,N*M,N*N*M*M,0);
    print_board(&a,&a,0,0,0,0);
    free(map);
    free(sol);
    //delete_board(&a);
}

void get_hint_test(){

	BOARD a,b;

	load_board("/specific/a/home/cc/students/csguests/nirsivan/Cproject/project/boards/test2.txt",&a,&b,3);
	print_board(&a,&b,0,0,0,0);
	int N = a.N;
	int M = a.M;
	printf("N=%d,M=%d",N,M);
	int *map = (int*)calloc(N*N*N*M*M*M,sizeof(int));
	int num_of_vars = map_maker(&a,map,N*M,N*M*N*M);
	double *sol = (double *)calloc(num_of_vars,sizeof(double));
	//srand(time(0));
	gurobi(&a,num_of_vars,map,0,sol);
	double *scores = (double *)calloc(N*M,sizeof(double));
    get_hint(map,sol,0,0,N*M,N*N*M*M,scores);
    print_board(&a,&a,0,0,0,0);
    print_array_double(scores,N*M);
    free(map);
    free(sol);
    free(scores);
    //delete_board(&a);
}


void test_command(){
	int i, num_of_commands, commands[] = {Solve, Set, Autofill, Reset, Autofill};
	BOARD game_board, fix_board;
	int nXm, mode, markErrors, args[3], execute, isValidBoard, isUpdatedBoard, numOfEmptyCells;
	float threshold;
	list command_list;
	char *path = "/specific/a/home/cc/students/csguests/nirsivan/Cproject/project/boards/test2.txt";
	threshold = 0;
	mode = INIT, markErrors = TRUE, isValidBoard = FALSE, isUpdatedBoard = FALSE, numOfEmptyCells = 0;


	initialize_puzzle(&game_board, &fix_board, &command_list);
	nXm = game_board.N*game_board.M;
	args[0] = 1;
	args[1] = 1;
	args[2] = 1;
  //exhust_backtrack_tester();
//	map_tester();
//	put_sol_test();

	//girobi_test();
//	get_hint_test();
	num_of_commands = 5;
	for (i=0; i<num_of_commands; i++) {
		char *commands[] = COMMAND_NAMES;
			printf("\ncommand: %s\n\n", commands[i]);
			int execute = execute_command(i, &game_board, &fix_board, &command_list, &markErrors, &mode, \
							&isValidBoard, &isUpdatedBoard, &nXm, &numOfEmptyCells, args, path, threshold);
			printf("result of execute_command is %d. markErrors = %d, mode = %d, isValidBoard = %d, isUpdatedBoard = %d, numOfEmptyCells = %d, nXm = %d\n", \
								execute, markErrors, mode, isValidBoard, isUpdatedBoard, numOfEmptyCells, nXm);
	}
}

void build_and_delete_board_test(){
	BOARD *board = init_board(5,5);
	set_element_to_board(board,1,1,6);
	set_element_to_board(board,1,5,9);
	set_element_to_board(board,1,5,0);
	set_element_to_board(board,1,3,8);
	set_element_to_board(board,2,10,12);


	print_board(board,board,0,0,0,0);
	delete_board(board);

}

void undo_and_redo_memory_test(){
	BOARD *b = init_board(2,3);
	set_element_to_board(b,1,1,1);
	list *list  = init_list(b);
	add_command(list,b,2);
	//add_command(list,b,7);
	//pop_command(list);
	//add_command(list,b,2);
	print_list(list,0);
	move_in_command_list(list,Undo);
	move_in_command_list(list,Undo);
	move_in_command_list(list,Undo);
	print_list(list,0);
	add_command(list,b,5);
	print_list(list,0);
	delete_list(list);
	delete_board(b);


}

void backtracking_memory_test(){
	stack *s = init_stack(1);
	push(s,5,3);
	push(s,5,2);
	int x;
	int y;
	int k;
	print_stack(s);
	pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);
	print_stack(s);

	pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);
	print_stack(s);

	pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);
	print_stack(s);

	pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);

	print_stack(s);
    pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);
	print_stack(s);

    pop(s,&x,&y,&k);
	printf("x=%d,y=%d,k=%d",x,y,k);
	print_stack(s);

	delete_stack(s);

	BOARD *b = init_board(2,3);
	printf("res = %d",exhaustive_backtracking(b));
	delete_board(b);
}

int main(){
	//build_and_delete_board_test();
	//undo_and_redo_memory_test();
	//backtracking_memory_test();
	return 0;
}

