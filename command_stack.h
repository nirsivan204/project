/*
 * command_stack.h
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
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
//	int num_of_commands;
}list;

list *init_list(BOARD *new_puzzle);
BOARD *get_curr_board(list* list);
char get_curr_command(node *node);
void delete_list(list* s);
void add_command(list *s, BOARD *board_after_command, int command_name);
//
//node* pop_command(list *s);
void print_list(list *s,int with_board);
//node* forward_current_command(list *s);
//
node* move_in_command_list(list *s, int command_name);
#endif /* COMMAND_STACK_H_ */
