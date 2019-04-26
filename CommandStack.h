/**
 * CommandStack module -  contains a list of the user's moves. This module supports the following functions:
 *
 * init_list    		- initializes a new command list.
 * get_curr_board 		- returns the current board state according to the list's pointer.
 * get_curr_command 	- returns the current command name used on board according to the list's pointer.
 * delete_list 			- deletes the list and frees all memory allocations.
 * add_command 			- adds a new node containing a move and adjust the list's pointer to it.
 * move_in_command_list - moves the list's pointer forward or backwards.
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "BoardHandle.h"

#ifndef COMMAND_STACK_H_
#define COMMAND_STACK_H_
typedef struct node{
	char command;
	BOARD *board_after_command;
	struct node *next;
	struct node *prev;
}node;

typedef struct list{
	node *first;
	node *current_command;
	BOARD *original_board;
}list;

/*
 *  this function initializes a new command list.
 *  @param new_puzzle	- a pointer to the current puzzle.
 *
 *  @return a new empty list.
 */
list *init_list(BOARD *new_puzzle);

/*
 *  this function returns the current board state according to the list's pointer.
 *  @param list	- a pointer to a list of the user's moves.
 *
 *  @return that board.
 */
BOARD *get_curr_board(list* list);

/*
 *  this function returns the current command name used on board according to the list's pointer.
 *  @param node	- a pointer to a node in the command list.
 *
 *  @return the node's character that represents that command.
 */
char get_curr_command(node *node);

/*
 *  this function deletes the list and frees all memory allocations.
 *  @param s	- a pointer to a list of the user's moves.
 */
void delete_list(list* s);

/*
 *  this function adds a new node containing a move and adjust the list's pointer to it.
 *  @param s					- a pointer to a list of the user's moves.
 *  @param board_after_command	- a pointer to the current puzzle.
 *  @param command_name			- a number a number that represents one of the 17 legal command names.
 */
void add_command(list *s, BOARD *board_after_command, int command_name);

/*
 *  this function moves the list's pointer forward or backwards.
 *  @param s					- a pointer to a list of the user's moves.
 *  @param command_name			- a number a number that represents the 'undo' command or the 'redo' command.
 *
 *  a short summary of each command's implementation:
 *  'undo'	- if the current node isn't NULL, the function moves the pointer one node backwards.
 *  'redo'	- if the current node isn't NULL, then if its next node isn't NULL the function moves the pointer one node forwards.
 *  		  Otherwise, if the current node is NULL, then if the list is empty it's an error, and otherwise the pointer moves to the first node in the list.
 *
 *  @return 1 if the pointer moves, 0 otherwise.
 */
int move_in_command_list(list *s , int command_name);
#endif /* COMMAND_STACK_H_ */
