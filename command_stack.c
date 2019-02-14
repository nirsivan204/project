/*
 * command_stack.c
 *
 *  Created on: 14 áôáø 2019
 *      Author: nir
 */
#include "command_stack.h"

list *init(){
	list *res = (list*) malloc(sizeof(list));
	res->first = NULL;
	res->current_command = NULL;
	res->num_of_commands = 0;
	return res;
}

void delete_nodes_recursivley(node *node){
	if(node != NULL){
		free(node->path);
		delete_nodes_recursivley(node->next);
		free(node->next);
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

void add_command(list *s,int command,float *args,char *path){
	node *current_command = s->current_command;
	node *element = (node*)malloc(sizeof(node));
	element->command = command;
	element->path = (char*)malloc(sizeof(char)*(strlen(path)+1));
	strcpy(element->path,path);
	memcpy(element->args,args,3*sizeof(float));
	element->prev = current_command;
	delete_next_nodes(current_command);
	current_command->next = element;
	element->next = NULL;
	s->num_of_commands++;
}

node *pop_command(list *s){
	node *res = s->current_command;
	s->current_command = res->prev;
	s->num_of_commands--;
	return res;
}
