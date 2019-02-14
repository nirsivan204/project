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

#ifndef COMMAND_STACK_H_
#define COMMAND_STACK_H_
typedef struct node{
	int command;
	int args[3];
	char *path;
	float threshold;
	struct node *next;
	struct node *prev;
}node;

typedef struct list{
	node *first;
	node *current_command;
	int num_of_commands;
}list;

list *init_list();
void delete_list(list* s);
void add_command(list *s,int command,int *args,char *path, float threshold);
node* pop_command(list *s);
void print_list(list *s);
node* forward_current_command(list *s);
#endif /* COMMAND_STACK_H_ */
