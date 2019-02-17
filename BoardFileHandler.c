/*
 * BoardFileHandler.c
 *
 *  Created on: 13 αταψ 2019
 *      Author: nir
 */
#include "BoardFileHandler.h"
#include "MainAux.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

//char ignore_white_spaces_in_string(char *string){
//	char res = fgetc(string);
//	while(is_white_space(res)){
//		res = fgetc(file);
//		continue;
//	}
//	return res;
//}

char ignore_white_spaces_in_file(FILE *file){
	char res = fgetc(file);
	while(is_white_space(res)){
		res = fgetc(file);
		continue;
	}
	return res;
}

void read_next_element(FILE *file,int *z,int *is_fixed){
	char c = ignore_white_spaces_in_file(file);
	*is_fixed = 0;
	*z = 0;
	if(!isdigit(c)){
		printf("error in file format");
	}
	while(isdigit(c)){
		*z = *z*10+(int)c-48;
		c = fgetc(file);
	}
	if(c == '.'){
		*is_fixed = 1;
	}else{
		if(!is_white_space(c)){
			printf("error in file format");
		}
	}
}




void load_board(char *path,BOARD *board, BOARD *fix_board){
	FILE *file;
	int N,M,x = 0 ,y = 0,z,is_fixed;
	file = fopen(path, "r");
	if(file == NULL){
		printf("error opening file");
		assert(0);
	}
	if(fscanf(file,"%d %d",&N,&M)<0){
		printf("error in file's format");
		assert(0);
	}
	printf("%d %d\n",N,M);
	*board = *init_board(N,M);
	*fix_board = *init_board(N,M);
	//print_board(board,fix_board);
	(fgetc(file));
	for(;y<M*N;y++){
		for(x=0;x<N*M;x++){
			read_next_element(file,&z,&is_fixed);
			set_element_to_board(board,x,y,z);
			set_element_to_board(fix_board,x,y,is_fixed);
		}
		fscanf(file,"\n");
	}
	//print_board(board,fix_board);
}

void save_board(char *path,BOARD *board,BOARD *fix_board,int mode){
	FILE *file;
	int i = 0,j = 0;
	file = fopen(path, "w");
	if(file == NULL){
		printf("error writing in file");
		assert(0);
	}
	fprintf(file,"%d %d\n",board->M,board->N);
	for (;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			fprintf(file,"%d",get_element_from_board(board,j,i));
			if(mode == 0 || get_element_from_board(fix_board,j,i)==1){
				fprintf(file,".");
			}else{
				if(j!=board->M*board->N-1){
					fprintf(file," ");
				}
			}
		}
		fprintf(file,"\n");
	}
}

