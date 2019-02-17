/*
 * Solver.c

 *
 *  Created on: 12 בדצמ 2018
 *      Author: nir
 */
#include "Solver.h"

/*
 * (for debug only)
 * this function prints an array
 * @param arr    - the array to print
 * @param length - how many cells to print
 *
 */

void print_array(int *arr,int length){ /*helper function for debug, prints an array*/
	int i;
	for (i = 0;i<length;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}

/*
 * this function return a random number in a desired range
 * @param range    - the range
 *
 * @return a random number between 0 to range-1 (include)
 */
int get_rand_number(int range){ /*returning a random number between 0 to range-1*/
	return rand()%range;
}

/*
 * this function deletes chosen number from the array of possible digits, and updating the array
 * ***for now, it is doing it naively because we decided
 *    that it is fast enough for small arrays and relatively small boards.
 *    in the future we will do it with a linked list and not arrays (for the deterministic case).
 *    one should notice, that for the random case,
 *    even linked list will not be more efficient because while deleting from the list will be faster
 *    (when we have the pointer to the item),
 *    getting to the right item (for retrieving or deleting) will take O(N*M)
 *    just like it is now.***
 *
 * @param legal_digits - the array of possible digits
 * @param index        - the index of the number is needed to be deleted
 * @param length       - the length of the array legal_digits
 *
 */
void delete_digit(int *legal_digits,int index, int length){
	int i;
	for(i=index;i<length-1;i++){
		legal_digits[i] = legal_digits[i+1];
	}
}

/*
 * this function is finding all legel digits (between 0 to N*M), and is putting them in the legel digits array.
 *
 * @param legal_digits - the array of possible digits
 * @param game_board   - the board to check
 * @param x            - the column of the cell to check
 * @param y            - the row of the cell to check
 *
 * @return the number of possible digits
 */

int find_legal_digits(int *legal_digits,BOARD *game_board,int x,int y){ /*finding all legel digits, and put them in the legel digits array. returning the num of possible digits*/
	int digit;
	int num_of_legal_digits = 0;
	for (digit = 1;digit<=game_board->N*game_board->M;digit++){
		if(is_valid_insertion(game_board,x,y,digit)==1){
			legal_digits[num_of_legal_digits] = digit;
			num_of_legal_digits++;
		}
	}
	return num_of_legal_digits;
}

/*
 * this function computes the next cordinates.
 *
 * @param x            - the column of the current cell
 * @param y            - the row of the current cell
 * @param next_x       - the column of the next cell
 * @param next_y       - the row of the next cell
 *
 * @return 1 if x,y are the right most and down most place in the board
 * 		   0 otherwise
 */

int compute_next_cordinates(int x, int y , int *next_x , int *next_y, int N, int M){
	if(x==N*M-1){
		*next_x = 0;
		if(y == N*M-1){
			return 1;
		}else{
			*next_y =y+1;
		}
	}else{
		*next_x = x+1;
		*next_y = y;
	}
	return 0;
}

/*
 * gets the next valid digit (randomly or deterministically)
 *
 * @param legal_digits         - the array of legal digits
 * @param num_of_legal_digits  - num of legal digits in the array
 * @param is_determin          - if 1 it will choose deterministically, if 0 it will choose randomly
 *
 * @return 1 if x,y are the right most and down most place in the board
 * 		   0 otherwise
 */

int get_valid_digit(int *legal_digits ,int num_of_legal_digits, int is_determin){ /**/
	int index = 0;
	int res = 0;
	if(num_of_legal_digits == 1){
		return legal_digits[0];
	}
	if(is_determin==0){ /*if randomly*/
		index = get_rand_number(num_of_legal_digits);/*get random index, else index =0*/
	}
	res = legal_digits[index];
	delete_digit(legal_digits,index,num_of_legal_digits);
	return res;
}

/*recursive method, to build the solved board: if is_determin == 0,
 * building randomly the board (first time initialize) ,
 * else, trying deterministicly to solve the board.
 *
 * @param solved_board - the board to build, at the end it will store the solution.
 * @param x            - the column
 * @param y            - the row
 * @param is_determin  - if 1 it will build deterministically, if 0 it will build randomly
 *
 * @return 1 if build was successful
 * 		   0 otherwise
 */

int build_board_helper(BOARD *solved_board,int x, int y, int is_determin){
	int next_x,next_y,digit,num_of_legal_digits, end_of_board = 0,valid = 0;
	int legal_digits[solved_board->N*solved_board->M];
	num_of_legal_digits = find_legal_digits(legal_digits,solved_board, x, y); /*find all legel digits for <x,y>*/
	end_of_board = compute_next_cordinates(x,y,&next_x,&next_y, solved_board->N, solved_board->M);/*find if it is the end of the board*/
	if(get_element_from_board(solved_board,x,y)!=0){  /*if this place has a value already, it means we are in deterministicly validation mode */
		if(end_of_board == 0 ){ /*if it is not the end of the board, continue with the next coordinates*/
			return build_board_helper(solved_board,next_x,next_y,is_determin);
		}
	}
	if(end_of_board == 1){ /*if it is the end of the board*/
		if(get_element_from_board(solved_board,x,y) == 0){ /*if this place doesnt have a value*/
			set_element_to_board(solved_board,x,y,legal_digits[0]); /*put the only value possible in solved_board*/
		}
		return 1;/*return it is solvable*/
	}
	if(num_of_legal_digits == 0){
		set_element_to_board(solved_board,x,y,0);
		return 0;
	}
	while (num_of_legal_digits>0 && valid == 0){
		digit = get_valid_digit(legal_digits , num_of_legal_digits , is_determin);
		set_element_to_board(solved_board,x,y,digit);
		valid = build_board_helper(solved_board,next_x,next_y,is_determin);
		num_of_legal_digits--;
	}
	if(valid == 0){
		set_element_to_board(solved_board,x,y,0);
		return 0;
	}
	return 1;
}

/*
 * choosing fix_num random places to fix in place,
 * and copying them from solved_board to the empty fix_board
 *
 * @param fix_num      - number of fixed cells
 * @param fix_board    - the board that will hold the fixed places at the end.
 * @param solved_board - the board that holds the solution.
 *
 */
void make_fix_board(int fix_num, BOARD *fix_board, BOARD *solved_board){/**/
	int counter = 0;
	int x;
	int y;
	while(counter<fix_num){
		x = get_rand_number(fix_board->N*fix_board->M);
		y = get_rand_number(fix_board->N*fix_board->M);
		if(get_element_from_board(fix_board,x,y)==0){
			set_element_to_board(fix_board,x,y,get_element_from_board(solved_board,x,y));
			counter++;
		}
	}
}

/*
 * wrapper function for the recursive method that builds the board
 * explanation in the .h file.
 *
 *
 */

int build_board(BOARD *board, int is_determin){
	return build_board_helper(board,0,0,is_determin);
}

/*
 * this function will zero 3 boards by calling the function zero_board 3 times.
 * @param board1 - the first board to initialize with zero.
 * @param board2 - the second board to initialize with zero.
 * @param board3 - the third board to initialize with zero.
 *
 */

void zero_boards(BOARD *board1, BOARD *board2, BOARD *board3){
	zero_board(board1);
	zero_board(board2);
	zero_board(board3);
}

/*
 * this function defined in the .h file.
 */

void initialize_puzzle (int fix_num, BOARD *game_board ,BOARD *fix_board, BOARD *solved_board){ /*initializing 3 boards: solved, fixed, and game. */
	zero_boards(game_board, fix_board, solved_board); /*zero all boards*/
	build_board(solved_board,0);/*find a random board*/
	make_fix_board(fix_num,fix_board,solved_board);/* fix "fix_num" of places in fix_board*/
	game_board = copy_board(fix_board); /* copy fix_board to game_board*/
	print_board(game_board,fix_board,1,1);
}

int find_next_empty_cell(BOARD *board,int *x,int *y){
	int i,j = *y;
	for (i=0;i<board->N*board->M;i++){
		for (j=0;j<board->N*board->M;j++){
			if(get_element_from_board(board,j,i)==0){
				*x = j;
				*y = i;
				return 1;
			}
		}
	}
	return -1;
}

int exhaustive_backtracking(BOARD *board){
	stack *s = init_stack(board->M*board->N);
	int x=0,y=0,digit,res=0;
	int pop_res;
	if(find_next_empty_cell(board,&x,&y)==-1){
		return 1;
	}
	push(s,x,y);
	while(s->top!=NULL){
		pop_res = pop(s,&x,&y,&digit);
		while(pop_res==1){
			set_element_to_board(board,x,y,0);
			pop_res = pop(s,&x,&y,&digit);
		}
		if(pop_res == -1){
			break;
		}
		if(!is_valid_insertion(board,x,y,digit)){
			continue;
		}
		set_element_to_board(board,x,y,digit);
		print_board(board,board,1,1);
		if(find_next_empty_cell(board,&x,&y)==-1){
			res++;
			set_element_to_board(board,x,y,0);
		}else{
			push(s,x,y);
		}
	}
	return res;
}



