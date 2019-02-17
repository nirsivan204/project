/*
 * backtracking_stack.h
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "BoardHandle.h"
#include "assert.h"
#ifndef BACKTRACKING_STACK_H_
#define BACKTRACKING_STACK_H_

typedef struct stack_element{
	//BOARD* board;
	int x,y;
	int next_digit;
	struct stack_element *next;
}stack_element;

typedef struct stack{
	int digits_range;
	stack_element *top;
}stack;

stack *init_stack(int digits_range);
void delete_stack(stack *s);
void push(stack *s ,int x, int y);
int pop(stack *s , int *x,int *y, int *next_digit);
void print_stack(stack *s);
#endif /* BACKTRACKING_STACK_H_ */
