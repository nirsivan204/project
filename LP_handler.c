/*
 * LP_handler.c
 *
 *  Created on: 12 במרס 2019
 *      Author: nir
 */
#include "LP_handler.h"

void mark_row(int *map,int row_offset,int k,int nXm){
	int j;
	for(j=0;j<nXm;j++){
		map[row_offset + nXm*j + k-1] = -1;
	}
}

void mark_col(int *map,int col_offset,int k,int nXm,int nXm_square){
	int i;
	for(i=0;i<nXm;i++){
		map[nXm_square*i + col_offset + k-1] = -1;
	}
}

void mark_cell(int *map,int row_offset,int col_offset,int nXm){
	int k;
	for(k=0;k<nXm;k++){
		map[row_offset + col_offset + k] = -1;
	}
}

void mark_block(int *map,int row,int col, int k,int n,int m,int nXm,int nXm_square){
	int first_row_of_block = (row/m)*m;
	int first_col_of_block = (col/n)*n;
	int row_offset,col_offset;
	int i,j;
	for(i=0;i<m;i++){
		row_offset = (first_row_of_block+i)*nXm_square;
		for (j=0;j<n;j++){
			col_offset = (first_col_of_block+j)*nXm;
			map[row_offset+col_offset+k-1] = -1;
		}
	}

}


int map_maker(BOARD *board,int *map, int nXm,int nXm_square){//assuming map is only zeros
	int i,j,k;
	int num_of_var = 0;
	int row_offset,col_offset;
	for(i=0;i<nXm;i++){
		row_offset = i*nXm_square;
		for(j=0;j<nXm;j++){
			col_offset = j*nXm;
			k=get_element_from_board(board,j,i);
			if(k!=0){
				mark_row(map,row_offset,k,nXm);
				mark_col(map,col_offset,k,nXm,nXm_square);
				mark_cell(map,row_offset,col_offset,nXm);
				mark_block(map,i,j,k,board->N,board->M,nXm,nXm_square);
			}
		}
	}
	for(i=0;i<nXm*nXm_square;i++){
		if(map[i]!=-1){
			map[i] = num_of_var;
			num_of_var++;
		}
	}
	return num_of_var;
}

int row_constraint(BOARD *board, int *map, int row, int digit,int *constraints, int nXm, int nXm_square){
	int j=0;
	int row_index = nXm_square*row;
	int map_index;
	if(is_valid_row(board,row,digit)){
		for (;j<nXm;j++){
			map_index = map[row_index+nXm*j+digit-1];
			if(map_index!=-1){
				constraints[map_index] = 1;
			}
		}
		return 1;
	}
	return 0;
}

int col_constraint(BOARD *board,int *map, int col, int digit,int *constraints, int nXm, int nXm_square){
	int i=0;
	int col_index = nXm*col;
	int map_index;
	if(is_valid_column(board,col,digit)){
		for (;i<nXm;i++){
			map_index = map[nXm_square*i+col_index+digit-1];
			if(map_index!=-1){
				constraints[map_index] = 1;
			}
		}
		return 1;
	}
	return 0;
}

int cell_constraint(BOARD *board,int *map, int row,int col,int *constraints, int nXm, int nXm_square){
	int k=1;
	int col_index = nXm*col;
	int row_index = nXm_square*row;
	int map_index;
	if(get_element_from_board(board,col,row)==0){
		for (;k<=nXm;k++){
				map_index = map[row_index+col_index+k-1];
				if(map_index!=-1){
					constraints[map_index] = 1;
				}
			}
			return 1;
	}
	return 0;
}

int block_constraint(BOARD *board,int *map,int block_row, int block_col,int digit,int *constraints,int nXm, int nXm_square){
	int map_index,i,j;
	int row_offset,col_offset;
	int m = board->M;
	int n = board->N;
	int first_row_index = block_row*m;
	int first_col_index = block_col*n;
	if(is_valid_block(board->blocks[block_row][block_col],digit)){
		for (i=0;i<m;i++){
			row_offset = (first_row_index+i)*nXm_square;
			for(j=0;j<n;j++){
				col_offset = (first_col_index+j)*nXm;
				map_index = map[row_offset + col_offset + digit-1];
				if(map_index!=-1){
					constraints[map_index] = 1;
				}
			}
		}
		return 1;
	}
	return 0;
}
