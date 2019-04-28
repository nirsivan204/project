#include "BacktrackingStack.h"

/*
 * this function initializes the stack for the algorithm. every element in the stack is corresopnding to a cell in the board.
 * each element has his coordinates <x,y> and next_digit which saves the next digit we want to check in the algorithm.
 * see explenation in the exhaustive_backtracking function.
 *
 * @param digits_range - the num of digits the algorithem needs to check for each cell.
 *
 * @return the initialized stack.
 *
 */
stack *init_stack(int digits_range){
	stack *res = (stack*)malloc(sizeof(stack));
	if(res == NULL){
		print_system_error(1,"Couldn't malloc enough memory for backtracking stack");
	}
	res->top = NULL;
	res->digits_range = digits_range;
	return res;
}
/*
 * this function deletes a stack and releasing all its memory.
 *
 * @param s - thestack to delete.
 *
 *
 */
void delete_stack(stack *s){
	if(s==NULL){
		return;
	}
	stack_element *element = s->top;
	while(element!=NULL){
		stack_element *temp = element->next;
		free(element);
		element = temp;
	}
	free(s);
}

/*
 * this function adds an element to the stack. every element in the stack is starting with
 * next_digit = 0.
 *
 * @param s - the stack.
 * @param x - the col of the cell this element represent.
 * @param y - the row of the cell this element represent.
 *
 */

void push(stack *s,int x, int y){
	stack_element *element =(stack_element*) malloc(sizeof(stack_element));
	if(element == NULL){
		print_system_error(1,"Couldn't malloc enough memory for backtracking stack element");
	}
	element->x = x;
	element->y = y;
	element->next_digit = 0;
	element->next = s->top;
	s->top = element;
}
/*
 * this function is releasing the memory of the element;
 *
 * @param element - the element to delete.
 *
 */
void delete_stack_element(stack_element *element){
	if(element !=NULL){
		free(element);
	}
}

/*
 * This function pops an element from the stack.
 * At first, it checks if this element's next_digit is larger then the stack's digits range.
 * If no, it will "pop" this element and "push" another one with the same <x,y> but next_digit of the new
 * element is larger then the previous by 1. (And return 0)
 * If yes, it will pop this element and delete it. (And return 1)
 *
 * @param s - the stack.
 * @param x - the col of the cell this popped element represent.
 * @param y - the row of the cell this popped element represent.
 * @param next_digit - the next_digit of the cell this popped element represent (after adding 1).
 *
 * return -1 if stack is empty or not initialized
 * 	       0 if the element was "popped and pushed" (next_digit<=digits range)
 * 	       1 if the element was popped.
 */
int pop(stack *s, int *x,int *y,int *next_digit){
	if(s == NULL||s->top == NULL){
		return -1; /*stack is empty or not initialized*/
	}
	*x = s->top->x;
	*y = s->top->y;
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

/*
 * (For debug only).
 * This function prints an element.
 *
 * @param element - the element to print.
 *
 */

void print_stack_element(stack_element *element){
	if(element == NULL){
		return;
	}
	printf("x = %d, y =%d\n", element->x,element->y);
	printf("digit: %d",element->next_digit);
	printf("\n|\n|\n\\/\n");
}

/*
 * (For debug only).
 * This function prints an entire stack.
 *
 * @param s - the stack to print.
 *
 */

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

/*
 * This function finds the next empty element in board. running from up to down, and left to right.
 *
 * @param board - the board.
 * @param x - the col of the next empty cell. (if exists)
 * @param y - the row of the next empty cell. (if exists)
 *
 * return  TRUE if there is an empty cell in board
 * 	       FALSE otherwise
 */

int find_next_empty_cell(BOARD *board,int *x,int *y){
	int i,j;
	for (i=0;i<board->N*board->M;i++){
		for (j=0;j<board->N*board->M;j++){
			if(get_element_from_board(board,j,i)==0){
				*x = j;
				*y = i;
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*
 * This function is using the exhausting backtracking algorithm to find
 * the number of possible solutions for the board.
 * At first it initializes a stack to hold the "recursion" steps.
 * If the board is full, returns 1 solution. (It cant be 0 cause the board is not erroronus because it is an error and this function will not be called)
 * Else, it pushes the first element for the first empty cell.
 * while the stack is not empty:
 *    it pops one element (each element needs to be popped NXM times to be really popped).
 *    while it was a "real" pop, it continues to pop until not.
 *    (this is the backtracking, going to the previous cells and empty all the exhusted cells)
 *    if the stack is empty, break because the algorithm was finished.
 *    else, if it is not a vlid insertion, move to the next digit/cell.
 *    if it is valid, insert it to the board. if the board is full, we found a possible solution, so res++, and set this cell to 0.
 *    if there are still empty cells, push the next element corresponds to the cell <x,y> into the stack.
 *    At the end it will return the number of possible solutions.
 *
 *
 * @param board - the board to run the algorithm on.
 *
 * return # of possible solutions
 */

int exhaustive_backtracking(BOARD *board){
	stack *s = init_stack(board->M*board->N);
	int m=0;
	int x=0,y=0,digit,res=0;
	int pop_res;
	if(!find_next_empty_cell(board,&x,&y)){
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
		if(!find_next_empty_cell(board,&x,&y)){
			res++;
			set_element_to_board(board,x,y,0);
		}else{
			push(s,x,y);
		}
	}
	delete_stack(s);
	return res;
}
