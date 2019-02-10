#include "BoardHandle.h"

/*
 * return a value of a cell in a block
 * @param block - the block of interest.
 * @param x     - the column of the cell in the board
 * @param y     - the row of the cell in the board
 *
 * @return the value of the cell
 */
int get_element_from_block(BLOCK block, int x, int y){ /*returning the element that in block, at cell <x,y>*/
	return block[y%N][x%M];
}

int get_element_from_board(BOARD board, int x,int y){/*returning the element that in board, at cell <x,y>*/
	return get_element_from_block(board[y/N][x/M],x,y);
}

/*
 * sets a value of a cell in a block
 * @param block - the block of interest.
 * @param x     - the column of the cell in the board
 * @param y     - the row of the cell in the board
 * @param z     - the value to put in the cell
 *
 */
void set_element_to_block(BLOCK block, int x, int y, int z){/*setting the element that in block, at cell <x,y> to z*/
	block[y%N][x%M] = z;
}

/*
 * this function defined in the .h file.
 */
void set_element_to_board(BOARD board, int x,int y,int z){/*setting the element that in board, at cell <x,y> to z*/
	set_element_to_block(board[y/N][x/M],x,y,z);
}

/*
 * this function defined in the .h file.
 */
void zero_board(BOARD board){/*setting all the elements in board to 0*/
	int i;
	int j;
	for (i=0;i<M*N;i++){
		for (j=0;j<M*N;j++){
			set_element_to_board(board,j,i,0);
		}
	}
}
/*
 * checks if this column in board contains z
 * @param board - the board of interest.
 * @param column- the column to check
 * @param z     - the value check
 * @return
 * 1 - if the column doesnt have the value z
 * 0 - otherwise
 */

int is_valid_column(BOARD board,int column,int z){/*checks if this column in  board contains z*/
	int index;
	for (index = 0; index < N*M; index++){/*for each row*/
		if (get_element_from_board(board, column, index) == z){ /*if the element in <row,column> is z */
			return 0;/*return not valid*/
		}
	}
	return 1;/*return valid*/
}

/*
 * checks if this row in board contains z
 * @param board - the board of interest.
 * @param row   - the column to check
 * @param z     - the value check
 * @return
 * 1 - if the row doesn't have the value z
 * 0 - otherwise
 */
int is_valid_row(BOARD board,int row,int z) {/*checks if this row in  board contains z*/
	int index;
	for (index = 0; index < N*M; index++){/*for each column*/
		if (get_element_from_board(board, index, row) == z){/*if the element in <row,column> is z */
			return 0;/*return not valid*/
		}
	}
	return 1;/*return valid*/
}

/*
 * checks if this block contains z
 * @param block - the board of interest.
 * @param z     - the value check
 * @return
 * 1 - if the blcok doesn't have the value z
 * 0 - otherwise
 */
int is_valid_block(BLOCK block,int z){/*checks if this block in  board contains z*/
	int index_col, index_row;
	for (index_col = 0; index_col < M; index_col++){ /*for each column of the block*/
		for (index_row = 0; index_row < N; index_row++){/*for each row of the block*/
			if (get_element_from_block(block, index_col, index_row) == z){/*if the element in <row,column> is z */
				return 0;/*return not valid*/
			}
		}
	}
	return 1;/*return valid*/
}

/*
 * this function defined in the .h file.
 */

int is_valid_insertion(BOARD board,int x,int y,int z) { /*returns if z can be set in cell <x,y> in board*/
	return is_valid_column(board,x,z) && is_valid_row(board,y,z) && is_valid_block(board[y/N][x/M],z);
}

/*
 * (for future use only)
 * this function prints the block
 * @param block       - the block needed to be printed.
 *
 */
void print_block(BLOCK block){ /*printing a given block*/
	int i;
	int j;
	for (i=0;i<N;i++){
		for (j=0;j<M;j++){
			printf("%d ", block[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


/*
 * this function prints a desired row from a block
 * @param block       - the block of interest.
 * @param fixed_block - the fixed block. the function needs it to print "." before fixed values, or " " before not-fixed values.
 * @param row         - the row (between 0 to N) needed to be printed
 */
void print_block_row(BLOCK block,BLOCK fixed_block,int row){ /*printing the row of block, by the printing rules in the header file*/
	int i;
	for (i=0;i<M;i++){/*for every col*/
		printf(" ");
		if(fixed_block[row][i]!=0){ /*if fixed, print "." else print " "*/
			printf(".");
		}else{
			printf(" ");
		}
		if(block[row][i]==0){ /*if empty, print " " else print the number*/
			printf(" ");
		}
		else{
			printf("%d",block[row][i]);
		}
	}
}

/*
 * this function defined in the .h file.
 */

void print_board(BOARD board, BOARD fixed_board){ /*printing the board, by the printing rules in the header file*/
	int block_row = 0 ;
	int block_col = 0 ;
	int row = 0;
	for (block_row = 0;block_row<M;block_row++){/*for each row of blocks in board*/
		printf(SEPARATOR_ROW);
		for(row = 0;row<N;row++){/*for each row in a block*/
			printf("|");
			for (block_col = 0;block_col<N;block_col++){/*for each block in the row*/
				print_block_row(board[block_row][block_col],fixed_board[block_row][block_col],row); /*print the row*/
				printf(" |");
			}
			printf("\n");
		}
	}
	printf(SEPARATOR_ROW);

}

/*this function is copying all the cells' value of one block to another block.
 * @param in_block - the block needed to be copied.
 * @param out_block - the block that will be a copy of in_board after the function call
 *
 */
void copy_Block(BLOCK in_block ,BLOCK out_block){ /*copy in_block to out_block*/
	int i=0;
	int j=0;
	for (i=0;i<N;i++){/*for each row of block*/
		for(j=0;j<M;j++){/*for each column of block*/
			out_block[i][j]=in_block[i][j];/*copy element*/
		}
	}
}

/*
 * this function defined in the .h file.
 */

void copy_board(BOARD in_board, BOARD out_board){/*copy in_board to out_board*/
	int i=0;
	int j=0;
	for (i=0;i<M;i++){/*for each row of blocks in board*/
		for(j=0;j<N;j++){/*for each column of blocks in board*/
			copy_Block(in_board[i][j],out_board[i][j]);/*copy block*/
		}
	}
}


