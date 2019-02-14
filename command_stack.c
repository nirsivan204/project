/*
 * command_stack.c
 *
 *  Created on: 14 αταψ 2019
 *      Author: nir
 */
#include "command_stack.h"

list *init_list(){
	list *res = (list*) malloc(sizeof(list));
	res->first = NULL;
	res->current_command = NULL;
	res->num_of_commands = 0;
	return res;
}

void delete_nodes_recursivley(node *node){
	if(node != NULL){
		free(node->path);
		delete_board(node->board_after_command);
		delete_nodes_recursivley(node->next);
		//free(node->next);
		free(node);
	}
}

void delete_next_nodes(node *node){
	if(node != NULL){
		delete_nodes_recursivley(node->next);
		node->next = NULL;
	}
}

void delete_list(list* s){
	delete_nodes_recursivley(s->first);
	free(s->current_command);
	free(s);
}

void add_command(list *s,int command,int *args,char *path ,float threshold, BOARD *board_after_command){
	node *current_command = s->current_command;
	node *element = (node*)malloc(sizeof(node));
	element->command = command;
	if(path!=NULL){
		element->path = (char*)malloc(sizeof(char)*(strlen(path)+1));
		strcpy(element->path,path);
	}else{
		element->path = NULL;
	}if(args!=NULL){
		memcpy(element->args,args,3*sizeof(int));
	}
	element->threshold = threshold;
	element->prev = current_command;
	element->board_after_command = copy_board(board_after_command);
	if(current_command!=NULL){
		delete_next_nodes(current_command);
		current_command->next = element;
	}else{
		s->first = element;
	}
	s->current_command = element;
	element->next = NULL;
	s->num_of_commands++;
}

node *pop_command(list *s){
	node *res = s->current_command;
	s->current_command = res->prev;
	s->num_of_commands--;
	return res;
}

node* forward_current_command(list *s){
	if(s == NULL){
		printf("list wasn't initialized");
		assert(0);
	}
	if(s->current_command == NULL){
		printf("list is empty");
		return NULL;
	}
	if(s->current_command->next == NULL){
		return NULL;
	}
	s->current_command = s->current_command->next;
	return s->current_command;
}

void print_node(node *node,int with_board){
	printf("command:%d\n",node->command);
	printf("args are %d %d %d\n",node->args[0],node->args[1],node->args[2]);
	if(node->path ==NULL){
		printf("path is NULL\n");
	}else{
		printf("%s\n",node->path);
	}
	printf("threshold is %f\n",node->threshold);
	if(with_board==1){
		if(node->board_after_command == NULL){
			printf("board is NULL\n");
		}else{
			print_board(node->board_after_command,node->board_after_command);
		}
	}
	printf("|\n|\n\\/\n");

}
void print_list(list *list, int with_board){
	node *node = list->first;
	while (node!=NULL){
		print_node(node,with_board);
		node=node->next;
	}
	printf("-------------\n");
}
