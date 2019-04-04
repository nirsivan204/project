/*
 * BoardFileHandler.c
 *
 *  Created on: 13 αταψ 2019
 *      Author: nir
 */
#include "BoardFileHandler.h"
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

int read_next_element(FILE *file,int *z,int *is_fixed,int N,int M){
	char c = ignore_white_spaces_in_file(file);
	*is_fixed = 0;
	*z = 0;
	if(!isdigit(c)){
		print_invalid_file_error(3);
		return FALSE;
	}
	while(isdigit(c)){
		*z = *z*10+(int)c-48;
		c = fgetc(file);
	}
	if(!is_in_range(*z,0,N*M)){
		print_invalid_file_error(3);
		return FALSE;
	}
	if(c == '.'){
		*is_fixed = 1;
	}else{
		if(!is_white_space(c)){
			print_invalid_file_error(3);
			return FALSE;
		}
	}
	return TRUE;
}

int close_file(FILE *file, int boolean) {
	fclose(file);
	return boolean;
}


int load_board(char *path,BOARD *board, BOARD *fix_board, int command_name){
	FILE *file;
	int N,M,x = 0 ,y = 0,z,is_fixed;
	file = fopen(path, "r");
	if(file == NULL){
		print_invalid_file_error(1);
		return FALSE;
	}
	if(fscanf(file,"%d %d",&N,&M)<0){
		print_invalid_file_error(3);
		return close_file(file, FALSE);
	}
	init_boards(board, fix_board, N, M);
	(fgetc(file));
	for(;y<M*N;y++){
		for(x=0;x<N*M;x++){
			if (!read_next_element(file,&z,&is_fixed,N,M)) {
//				delete_boards(board, fix_board);
				return close_file(file, FALSE);
			}
			set_element_to_board(board,x,y,z);
			if (command_name == Solve) {
				set_element_to_board(fix_board,x,y,is_fixed);
			}
		}
		fscanf(file,"\n");
	}
	return close_file(file, TRUE);
}

int save_board(char *path,BOARD *board,BOARD *fix_board,int mode){
	FILE *file;
	int i = 0,j = 0, val;
	file = fopen(path, "w");
	if(file == NULL){
		print_invalid_file_error(2);
		return FALSE;
	}
	fprintf(file,"%d %d\n",board->N,board->M);
	for (;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
			val = get_element_from_board(board,j,i);
			fprintf(file,"%d", val);
			if(val > 0 && (mode == EDIT || get_element_from_board(fix_board,j,i)==FIXED)){
				fprintf(file,". ");
			}else{
				if(j!=board->M*board->N-1){
					fprintf(file," ");
				}
			}
		}
		fprintf(file,"\n");
	}
	return close_file(file, TRUE);
}

