/*
 * BoardFileHandler.c
 *
 *  Created on: 13 αταψ 2019
 *      Author: nir
 */
#include "BoardFileHandler.h"
#include "assert.h"
#include <stdlib.h>
#include <stdio.h>

void load_board(char *path,BOARD *board, BOARD *fix_board){
	FILE *file;
	int N,M,x = 0 ,y = 0,z;
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
	BOARD *res = NULL;
	res = init_board(N,M);
	print_board(res,res);
	(fgetc(file));
	for(;y<M*N;y++){
		for(x=0;x<N*M;x++){
			fscanf(file,"%d ",&z);
			set_element_to_board(res,x,y,z);
		}
		fscanf(file,"\n");
	}
	print_board(res,res);
	delete_board(res);
}

