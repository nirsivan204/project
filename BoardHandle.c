#include "BoardHandle.h"

BLOCK* init_block(int N, int M) {
	BLOCK* result;
	int i = 0;
	result = (BLOCK*)malloc(sizeof(BLOCK));
	result->M = M;
	result->N = N;
	result->values = (int**)malloc(sizeof(int*)*M);
	for(;i<M;i++) {
		result->values[i] = (int*)calloc(N, sizeof(int));
	}
	return result;
}

BOARD* init_board(int N, int M) {
	BOARD* result;
	int i = 0;
	int j = 0;
	result = (BOARD*)malloc(sizeof(BOARD));
	result->M = M;
	result->N = N;
	result->blocks = (BLOCK***)malloc(sizeof(BLOCK**)*N);
	for(;i<N;i++) {
		result->blocks[i] = (BLOCK**)malloc(sizeof(BLOCK*)*M);
		for(j=0;j<M;j++){
			result->blocks[i][j] = init_block(N,M);
		}
	}
	for(i=0; i<4*N*M+2*N+1;i++){
		result->line_seperator[i] = '-';
	}
	result->line_seperator[i] = '\n';
	result->line_seperator[i+1]='\0';
	return result;
}

void delete_block(BLOCK* block) {
	int i = 0;
	for(;i<block->M;i++) {
		free(block->values[i]);
	}
	free(block->values);
	free(block);
}

void delete_board(BOARD* board) {
	if(board == NULL){
		printf("board is NULL");
		return;
	}
	int i = 0;
	int j = 0;
	for(;i<board->N;i++) {
		for(j=0;j<board->M;j++){
			delete_block(board->blocks[i][j]);
		}
		free(board->blocks[i]);
	}
	free(board->blocks);
	free(board);
}

/*
 * return a value of a cell in a block
 * @param block - the block of interest.
 * @param x     - the column of the cell in the board
 * @param y     - the row of the cell in the board
 *
 * @return the value of the cell
 */
int get_element_from_block(BLOCK *block, int x, int y){ /*returning the element that in block, at cell <x,y>*/
	return block->values[y%block->M][x%block->N];
}

int get_element_from_board(BOARD *board, int x,int y){/*returning the element that in board, at cell <x,y>*/
	return get_element_from_block(board->blocks[y/board->M][x/board->N],x,y);
}

/*
 * sets a value of a cell in a block
 * @param block - the block of interest.
 * @param x     - the column of the cell in the board
 * @param y     - the row of the cell in the board
 * @param z     - the value to put in the cell
 *
 */
void set_element_to_block(BLOCK *block, int x, int y, int z){/*setting the element that in block, at cell <x,y> to z*/
	 block->values[y%block->M][x%block->N] = z;
}

/*
 * this function defined in the .h file.
 */
void set_element_to_board(BOARD *board, int x,int y,int z){/*setting the element that in board, at cell <x,y> to z*/
	set_element_to_block(board->blocks[y/board->M][x/board->N],x,y,z);
}

/*
 * this function defined in the .h file.
 */
void zero_board(BOARD *board){/*setting all the elements in board to 0*/
	int i;
	int j;
	for (i=0;i<board->M*board->N;i++){
		for (j=0;j<board->M*board->N;j++){
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

int is_valid_column(BOARD *board,int column,int z){/*checks if this column in  board contains z*/
	int index;
	for (index = 0; index < board->N*board->M; index++){/*for each row*/
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
int is_valid_row(BOARD *board,int row,int z) {/*checks if this row in  board contains z*/
	int index;
	for (index = 0; index < board->N*board->M; index++){/*for each column*/
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
int is_valid_block(BLOCK *block,int z){/*checks if this block in  board contains z*/
	int index_col= 0 , index_row = 0;
	for (index_col = 0; index_col < block->N; index_col++){ /*for each column of the block*/
		for (index_row = 0; index_row < block->M; index_row++){/*for each row of the block*/
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

int is_valid_insertion(BOARD *board,int x,int y,int z) { /*returns if z can be set in cell <x,y> in board*/
	return is_valid_column(board,x,z) && is_valid_row(board,y,z) && is_valid_block(board->blocks[y/board->M][x/board->N],z);
}

/*
 * (for future use only)
 * this function prints the block
 * @param block       - the block needed to be printed.
 *
 */
void print_block(BLOCK *block){ /*printing a given block*/
	int i;
	int j;
	for (i=0;i<block->M;i++){
		for (j=0;j<block->N;j++){
			printf("%d ", block->values[i][j]);
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
void print_block_row(BLOCK *block,BLOCK *fixed_block,int row,int mark_errors,int mode){ /*printing the row of block, by the printing rules in the header file*/
	int i;
	for (i=0;i<block->N;i++){/*for every col*/
		printf(" ");
		if(block->values[row][i]==0){ /*if empty, print " " else print the number*/
			printf(" ");
		}
		else{
			printf("%d",block->values[row][i]);
		}
		if(fixed_block->values[row][i]==1 && mode != EDIT){ /*if fixed, print "."*/
			printf(".");
		}else{
			if(fixed_block->values[row][i]==2 && (mode == EDIT || mark_errors==1)){ /*if erroneous, print "*" */
				printf("*");
			}else{
				printf(" ");
			}
		}
	}
}

/*
 * this function defined in the .h file.
 */

void print_board(BOARD *board, BOARD *fixed_board,int mark_errors,int mode){ /*printing the board, by the printing rules in the header file*/
	int block_row = 0 ;
	int block_col = 0 ;
	int row = 0;
	for (block_row = 0;block_row<board->N;block_row++){/*for each row of blocks in board*/
		printf("%s",board->line_seperator);
		for(row = 0;row<board->M;row++){/*for each row in a block*/
			printf("|");
			for (block_col = 0;block_col<board->M;block_col++){/*for each block in the row*/
				print_block_row(board->blocks[block_row][block_col],fixed_board->blocks[block_row][block_col],row , mark_errors,mode); /*print the row*/
				printf(" |");
			}
			printf("\n");
		}
	}
	printf("%s",board->line_seperator);

}

/*this function is copying all the cells' value of one block to another block.
 * @param in_block - the block needed to be copied.
 * @param out_block - the block that will be a copy of in_board after the function call
 *
 */
BLOCK *copy_Block(BLOCK *in_block){ /*return a copy of in_block*/
	BLOCK *res;
	res = (BLOCK*)malloc(sizeof(BLOCK));
	res->M = in_block->M;
	res->N = in_block->N;
	res->values = (int**)malloc(res->M*sizeof(int*));
	int i=0;
	int j=0;
	for (i=0;i<in_block->M;i++){/*for each row of block*/
		res->values[i] = (int*)malloc(res->N*sizeof(int));
		for(j=0;j<in_block->N;j++){/*for each column of block*/
			res->values[i][j]=in_block->values[i][j];/*copy element*/
		}
	}
	return res;
}

/*
 * this function defined in the .h file.
 */

BOARD *copy_board(BOARD *in_board){/*return a copy of in_board*/
	if(in_board == NULL){
		return NULL;
	}
	BOARD *res = (BOARD*)malloc(sizeof(BOARD));
	res->M = in_board->M;
	res->N = in_board->N;
	strcpy(res->line_seperator,in_board->line_seperator);
	res->blocks = (BLOCK***)malloc(res->N*sizeof(BLOCK**));
	int i=0;
	int j=0;
	for (i=0;i<in_board->N;i++){/*for each row of blocks in board*/
		res->blocks[i] = (BLOCK**)malloc(res->M*sizeof(BLOCK*));
		for(j=0;j<in_board->M;j++){/*for each column of blocks in board*/
			res->blocks[i][j] = copy_Block(in_board->blocks[i][j]);/*copy block*/
		}
	}
	return res;
}


