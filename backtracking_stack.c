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

