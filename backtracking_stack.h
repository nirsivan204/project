/*
 * backtracking_stack.h
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "BoardHandle.h"

#ifndef BACKTRACKING_STACK_H_
#define BACKTRACKING_STACK_H_

typedef struct stack_element{ /*this struct is defining an element for the stack */
	int x,y;/*x is the col, y is the row of the element in the current call*/
	int next_digit;/*the next digit the algorithem needs to check when getting to this element */
	struct stack_element *next; /*the next element in the stack */
}stack_element;

typedef struct stack{/*this struct is defining an stack for the exhausting backtracking algorithm */
	int digits_range; /*equals to NXM, defining the range of numbers the algorithm need to check*/
	stack_element *top;/*a pointer to the first element of the stack */
}stack;


/*
 * this function is using the exhausting backtracking algorithm to find
 * the number of possible solutions for the board.
 * full explanation in the .c file.
 *
 * @param board - the board to run the algorithm on.
 *
 * @return # of possible solutions for the board;
 *
 */
int exhaustive_backtracking(BOARD *board);
#endif /* BACKTRACKING_STACK_H_ */
