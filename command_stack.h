/*
 * command_stack.h
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
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

list *init_list(BOARD *new_puzzle);
BOARD *get_curr_board(list* list);
char get_curr_command(node *node);
void delete_list(list* s);
void add_command(list *s, BOARD *board_after_command, int command_name);
void print_list(list *s,int with_board);
int move_in_command_list(list *s , int command_name);
#endif /* COMMAND_STACK_H_ */
