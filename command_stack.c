/*
 * command_stack.c
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "command_stack.h"

list *init_list(BOARD *new_puzzle){
	list *res = (list*) malloc(sizeof(list));
	res->first = NULL;
	res->current_command = NULL;
	res->original_board = copy_board(new_puzzle);
	return res;
}

BOARD *get_curr_board(list* list) {
	return list->current_command == NULL ? list->original_board : list->current_command->board_after_command;
}

char get_curr_command(node *node) {
	return node == NULL ? 'o' : node->command;
}

void delete_nodes_recursivley(node *node){
	if(node != NULL){
		delete_board(node->board_after_command);
		delete_nodes_recursivley(node->next);
		free(node);
		return;
	}
}

void delete_next_nodes(node *node){
	if(node != NULL){
		delete_nodes_recursivley(node->next);
		node->next = NULL;
	}
}

void delete_list(list* s){
	delete_board(s->original_board);
	delete_nodes_recursivley(s->first);
	free(s);
}

void add_command(list *s, BOARD *board_after_command, int command_name){
	node *current_command = s->current_command;
	node *element = (node*)malloc(sizeof(node));
	switch (command_name) {
	case Set: element->command = 's'; break;
	case Autofill: element->command = 'a'; break;
	case Guess: element->command = 'g'; break;
	default: element->command = 'n'; /* command is 'generate'*/
	}
	element->prev = current_command;
	element->board_after_command = copy_board(board_after_command);
	if(current_command!=NULL){
		delete_next_nodes(current_command);
		current_command->next = element;
	}else{
		delete_nodes_recursivley(s->first);
		s->first = element;
	}
	s->current_command = element;
	element->next = NULL;
}
/*
node *pop_command(list *s){
//	node *res = s->current_command;
	s->current_command = s->current_command->prev;
//	s->num_of_commands--;
//	return res;
	return s->current_command;
}

node* forward_current_command(list *s){
//	if(s == NULL){
//		printf("list wasn't initialized");
//		assert(0);
//	}
//	if(s->current_command == NULL){
//		printf("list is empty");
//		return NULL;
//	}
//	if(s->current_command->next == NULL){
//		return NULL;
//	}
//	s->current_command = s->current_command->next;
//	return s->current_command;
	s->current_command = s->current_command == NULL ? s->first : s->current_command->next;
	return s->current_command;
}*/

node* move_in_command_list(list *s, int command_name) {
	if (command_name == Undo) {
		if(s->current_command!=NULL){
			s->current_command = s->current_command->prev;
		}else{
			printf("can't undo, board in initial state\n");
		}
	}
	else { /*command is 'redo'*/
		if(s->current_command->next != NULL){
			s->current_command = s->current_command == NULL ? s->first : s->current_command->next;
		}
		else{
			printf("can't redo, no next commands\n");
		}
	}
	return s->current_command;
}

void print_node(node *node,int with_board){
//	printf("command:%d\n",node->command);
//	printf("args are %d %d %d\n",node->args[0],node->args[1],node->args[2]);
//	printf("threshold is %f\n",node->threshold);
	if(with_board==1){
		if(node->board_after_command == NULL){
			printf("board is NULL\n");
		}else{
			test_print_board(node->board_after_command,node->board_after_command);
		}
	}
	printf("command: %c\n", node->command);
	printf("|\n|\n\\/\n");

}
void print_list(list *list, int with_board){
	node *node = list->first;
	while (node!=NULL){
		if (node == list->current_command) {
			printf("current:\n");
		}else{
			if(list->current_command==NULL){
				printf("current state is the initial board\n");
			}
		}
		print_node(node,with_board);
		node=node->next;
	}
	printf("-------------\n");
}
