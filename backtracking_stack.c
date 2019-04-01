/*
 * backtracking_stack.c
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "backtracking_stack.h"
stack *init_stack(int digits_range){
	stack *res = (stack*)malloc(sizeof(stack));
	res->top = NULL;
	res->digits_range = digits_range;
	return res;
}

void delete_stack(stack *s){
	if(s==NULL){
		printf("stack is not initizalized");
		return;
	}
	stack_element *element = s->top;
	while(element!=NULL){
		//delete_board(element->board);
		stack_element *temp = element->next;
		free(element);
		element = temp;
	}
	free(s);
}

void push(stack *s,int x, int y){
	stack_element *element =(stack_element*) malloc(sizeof(stack_element));
	//element->board = copy_board(board);
	element->x = x;
	element->y = y;
	element->next_digit = 0;
	element->next = s->top;
	s->top = element;
}

void delete_stack_element(stack_element *element){
	//delete_board(element->board);
	free(element);
}

int pop(stack *s, int *x,int *y,int *next_digit){
	if(s == NULL){
		printf("stack is not initialized");
		assert(0);
	}
	if(s->top == NULL){
//		printf("stack is empty");
		return -1;
	}
	*x = s->top->x;
	*y = s->top->y;
	//*board = s->top->board;
	s->top->next_digit++;
	if(s->top->next_digit<=s->digits_range){
		*next_digit = s->top->next_digit;
		return 0;
	}else{
		stack_element *temp = s->top->next;
		delete_stack_element(s->top);
		s->top = temp;
		return 1;
	}
}
void print_stack_element(stack_element *element){
	if(element == NULL){
		return;
	}
	printf("x = %d, y =%d\n", element->x,element->y);
	printf("digit: %d",element->next_digit);
	printf("\n|\n|\n\\/\n");
}

void print_stack(stack *s){
	if(s == NULL){
		printf("stack is not initialized");
	}
	stack_element *element = s->top;
	while(element!=NULL){
		print_stack_element(element);
		element=element->next;
	}
	printf("---------------\n");
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
//		test_print_board(board,board);
		if(find_next_empty_cell(board,&x,&y)==-1){
			res++;
			set_element_to_board(board,x,y,0);
		}else{
			push(s,x,y);
		}
	}
	return res;
}
