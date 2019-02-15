/*
 * backtracking_stack.h
 *
 *  Created on: 14 ���� 2019
 *      Author: nir
 */
#include "BoardHandle.h"
#include "assert.h"
#ifndef BACKTRACKING_STACK_H_
#define BACKTRACKING_STACK_H_

typedef struct stack_element{
	BOARD* board;
	int x,y;
	struct stack_element *next;
}stack_element;

typedef struct stack{
	stack_element *top;
}stack;

stack *init_stack();
void delete_stack(stack *s);
void push(stack* s,BOARD *board, int x, int y);
int pop(stack *s , BOARD **board, int *x,int *y);
void print_stack(stack *s);
#endif /* BACKTRACKING_STACK_H_ */
