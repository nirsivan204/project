/*
 * command_stack.c
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "CommandStack.h"

/*
 *  this function initializes a new command list.
 *  @param new_puzzle	- a pointer to the current puzzle.
 *
 *  @return a new empty list.
 */
list *init_list(BOARD *new_puzzle){
	list *res = (list*) malloc(sizeof(list));
	if(res == NULL){
		print_system_error(1,"Couldn't malloc enough memory for command list");
	}
	res->first = NULL;
	res->current_command = NULL;
	res->original_board = copy_board(new_puzzle);
	return res;
}

/*
 *  this function returns the current board state according to the list's pointer.
 *  @param list	- a pointer to a list of the user's moves.
 *
 *  @return that board.
 */
BOARD *get_curr_board(list* list) {
	return list->current_command == NULL ? list->original_board : list->current_command->board_after_command;
}

/*
 *  this function returns the current command name used on board according to the list's pointer.
 *  @param node	- a pointer to a node in the command list.
 *
 *  @return the node's character that represents that command.
 */
char get_curr_command(node *node) {
	return node == NULL ? 'o' : node->command;
}

/*
 *  this function deletes the all nodes from a given node recursively and frees their memory allocation.
 *  @param node	- a pointer to a list node.
 */
void delete_nodes_recursively(node *node){
	if(node != NULL){
		delete_board(node->board_after_command);
		delete_nodes_recursively(node->next);
		free(node);
		return;
	}
}

/*
 *  this function deletes the all nodes from a given node.
 *  @param node	- a pointer to a list node.
 */
void delete_next_nodes(node *node){
	if(node != NULL){
		delete_nodes_recursively(node->next);
		node->next = NULL;
	}
}

/*
 *  this function deletes the list and frees all memory allocations.
 *  @param s	- a pointer to a list of the user's moves.
 */
void delete_list(list* s){
	if(s != NULL){
		delete_board(s->original_board);
		delete_nodes_recursively(s->first);
		free(s);
	}
}

/*
 *  this function adds a new node containing a move and adjust the list's pointer to it.
 *  @param s					- a pointer to a list of the user's moves.
 *  @param board_after_command	- a pointer to the current puzzle.
 *  @param command_name			- a number a number that represents one of the 17 legal command names.
 */
void add_command(list *s, BOARD *board_after_command, int command_name){
	node *current_command = s->current_command;
	node *element = (node*)malloc(sizeof(node));
	if(element == NULL){
		print_system_error(1,"Couldn't malloc enough memory for command list node");
	}
	switch (command_name) {
	case Set: element->command = 's'; break;
	case Autofill: element->command = 'a'; break;
	case Guess: element->command = 'g'; break;
	default: element->command = 'n'; /* command is 'generate'*/
	}
	element->next=NULL;
	element->prev = current_command;
	element->board_after_command = copy_board(board_after_command);
	if(current_command!=NULL){
		delete_next_nodes(current_command);
		current_command->next = element;
	}else{
		delete_nodes_recursively(s->first);
		s->first = element;
	}
	s->current_command = element;
	element->next = NULL;
}

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
int move_in_command_list(list *s,int command_name) {
	if (command_name == Undo) {
		if(s->current_command!=NULL){
			s->current_command = s->current_command->prev;
			return TRUE;
		}
		return FALSE;
	}
	/*else - command is 'redo'*/
	if(s->current_command != NULL){
		if(s->current_command->next != NULL){
			s->current_command = s->current_command->next;
			return TRUE;
		}
		return FALSE;
	}
	/*else - s->current_command == NULL*/
	if(s->first==NULL){
		return FALSE;
	}
	s->current_command = s->first;
	return TRUE;
}

/* for debug only */
void print_node(node *node,int with_board){
	if(node == NULL){
		return;
	}
	if(with_board==1){
		if(node->board_after_command == NULL){
			printf("board is NULL\n");
		}else{
			/*test_print_board(node->board_after_command,node->board_after_command);*/
		}
	}
	printf("command: %c\n", node->command);
	printf("|\n|\n\\/\n");
}

/* for debug only */
void print_list(list *list, int with_board){
	if(list==NULL){
		return;
	}
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
