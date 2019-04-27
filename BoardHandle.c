#include "BoardHandle.h"

/*
 * this function initializes an empty block with M rows and N columns of elements, and returns it.
 *
 * @param N - # of cols of elements.
 * @param M - # of rows of elements.
 * @return the empty block.
 *
 */
BLOCK* init_block(int N, int M) {
	BLOCK* result;
	int i = 0;
	result = (BLOCK*)malloc(sizeof(BLOCK));
	if (result == NULL) {
		print_system_error(1, "couldn't build board");
	}
	result->M = M;
	result->N = N;
	result->values = (int**)malloc(sizeof(int*)*M);
	if (result->values == NULL) {
		print_system_error(1, "couldn't build board");
	}
	for(;i<M;i++) {
		result->values[i] = (int*)calloc(N, sizeof(int));
		if (result->values[i] == NULL) {
			print_system_error(1, "couldn't build board");
		}
	}
	return result;
}

/*
 * this function initializes an empty board with N rows and M columns of blocks, and returns it.
 * @param N - # of rows of blocks.
 * @param M - # of cols of blocks.
 * @return the empty board.
 */

BOARD* init_board(int N, int M) {
	BOARD* result;
	int i = 0;
	int j = 0;
	result = (BOARD*)malloc(sizeof(BOARD));
	if (result == NULL) {
		print_system_error(1, "couldn't build board");
	}
	result->M = M;
	result->N = N;
	result->blocks = (BLOCK***)malloc(sizeof(BLOCK**)*N);
	if (result->blocks == NULL) {
		print_system_error(1, "couldn't build board");
	}
	for(;i<N;i++) {
		result->blocks[i] = (BLOCK**)malloc(sizeof(BLOCK*)*M);
		if (result->blocks[i] == NULL) {
			print_system_error(1, "couldn't build board");
		}
		for(j=0;j<M;j++){
			result->blocks[i][j] = init_block(N,M);
			if (result->blocks[i][j] == NULL) {
				print_system_error(1, "couldn't build board");
			}
		}
	}
	for(i=0; i<(3*N+2)*M+1;i++){ /*4*N*M+M+1*/
		result->line_seperator[i] = '-';
	}
	result->line_seperator[i] = '\n';
	result->line_seperator[i+1]='\0';
	return result;
}
/*
 * this function defined in the .h file.
 */
void init_boards(BOARD** board1, BOARD** board2, int N, int M) {
	*board1 = init_board(N,M);
	*board2 = init_board(N,M);
}
/*
 * this function releasing all the memory allocated for a block.
 *
 * @param block - the block needed to be deleted.
 */
void delete_block(BLOCK* block) {
	int i = 0;
	if(block == NULL){
		return;
	}
	if(block->values!=NULL){
		for(;i<block->M;i++) {
			if(block->values[i]!=NULL){
				free(block->values[i]);
			}
		}
		free(block->values);
	}
	free(block);
}
/*
 * this function defined in the .h file.
 */
void delete_board(BOARD* board) {
	int i = 0;
	int j = 0;
	if(board == NULL){
		return;
	}
	if(board->blocks!=NULL){
		for(;i<board->N;i++) {
			if(board->blocks[i]!=NULL){
				for(j=0;j<board->M;j++){
					delete_block(board->blocks[i][j]);
				}
				free(board->blocks[i]);
			}
		}
		free(board->blocks);
	}
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
/*
 * this function defined in the .h file.
 */
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
int is_valid_column(BOARD *board,int column,int z){
	int index;
	for (index = 0; index < board->N*board->M; index++){/*for each row*/
		if (get_element_from_board(board, column, index) == z){ /*if the element in <row,column> is z */
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * this function defined in the .h file.
 */
int is_valid_row(BOARD *board,int row,int z) {
	int index;
	for (index = 0; index < board->N*board->M; index++){/*for each column*/
		if (get_element_from_board(board, index, row) == z){/*if the element in <row,column> is z */
			return FALSE;
		}
	}
	return TRUE;
}

/*
 * this function defined in the .h file.
 */
int is_valid_block(BLOCK *block,int z){
	int index_col, index_row;
	for (index_col = 0; index_col < block->N; index_col++){ /*for each column of the block*/
		for (index_row = 0; index_row < block->M; index_row++){/*for each row of the block*/
			if (get_element_from_block(block, index_col, index_row) == z){/*if the element in <row,column> is z */
				return FALSE;
			}
		}
	}
	return TRUE;
}

/*
 * this function defined in the .h file.
 */
int is_valid_insertion_to_empty_cell(BOARD *board,int x,int y,int z) {
	return is_valid_column(board,x,z) && is_valid_row(board,y,z) && is_valid_block(board->blocks[y/board->M][x/board->N],z);
}

/*
 * this function defined in the .h file.
 */
int is_valid_insertion(BOARD *board,int x,int y,int z) {
	/* to simplify the validation, we prefer using an empty cell while examine the relevant row, column and block. */
	int val, is_valid;
	if (z == 0) {
		return TRUE;
	}
	val = get_element_from_board(board, x, y);
	if (val > 0) { /* non-empty cell: the value is temporarily removed and re-inserted after the validation. */
		set_element_to_board(board, x, y, 0);
		is_valid = is_valid_insertion_to_empty_cell(board,x,y,z);
		set_element_to_board(board, x, y, val);
		return is_valid;
	}
	return is_valid_insertion_to_empty_cell(board,x,y,z);
}

/*
 * this function makes sure the marking-board is updated (every cell in the game-board has the relevant mark in the marking-board)
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 */
void update_erroneous_cells(BOARD *board, BOARD *marking_board, int* isValidBoard, int* isUpdatedBoard) {
	int i,j,status;
	if (*isUpdatedBoard == FALSE) { /* marking_board needs updating */
		*isValidBoard = TRUE;
		for (i=0;i<board->M*board->N;i++){
			for (j=0;j<board->M*board->N;j++){
				status = get_element_from_board(marking_board,i,j);
				if (is_valid_insertion(board, i, j, get_element_from_board(board,i,j))) { /* cell has a valid value */
					if (status == ERRONEOUS) {
						set_element_to_board(marking_board,i,j, NORMAL);
					}
				}
				else { /* cell has an erroneous value */
					*isValidBoard = FALSE;
					if (status == NORMAL) {
						set_element_to_board(marking_board,i,j, ERRONEOUS);
					}
				}
			}
		}
		*isUpdatedBoard = TRUE; /* marking_board is now updated */
	}
}

/*
 * this function defined in the .h file.
 */
int is_valid_board(BOARD *board, BOARD *marking_board, int* isValidBoard, int* isUpdatedBoard) {
	update_erroneous_cells(board, marking_board, isValidBoard, isUpdatedBoard);
	return *isValidBoard;
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
 * @param marking_block - the marking block. the function needs it to print "." before fixed values, or " " before not-fixed values.
 * @param row         - the row (between 0 to N) needed to be printed
 */
void print_block_row(BLOCK *block,BLOCK *marking_block,int row,int mark){ /*printing the row of block, by the printing rules in the header file*/
	int i;
	for (i=0;i<block->N;i++){/*for every col*/
		printf(" ");
		if(block->values[row][i]==0){ /*if empty, print " " else print the number*/
			printf(" ");
		}
		else{
			printf("%d",block->values[row][i]);
		}
		if(marking_block->values[row][i]==FIXED){ /*if fixed, print "."*/
			printf(".");
		}else{
			if(marking_block->values[row][i]==ERRONEOUS && mark == TRUE){ /*if erroneous, print "*" */
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
void print_board(BOARD *board, BOARD *marking_board,int mark_errors,int mode, int* isValidBoard, int* isUpdatedBoard){ /*printing the board, by the printing rules in the header file*/
	int block_row = 0 ;
	int block_col = 0 ;
	int row = 0;
	int mark;
	if(board == NULL){
		return;
	}
	mark = mode == EDIT || mark_errors==TRUE;
	if (mark == TRUE) {
		update_erroneous_cells(board, marking_board, isValidBoard, isUpdatedBoard);
	}
	for (block_row = 0;block_row<board->N;block_row++){/*for each row of blocks in board*/
		printf("%s",board->line_seperator);
		for(row = 0;row<board->M;row++){/*for each row in a block*/
			printf("|");
			for (block_col = 0;block_col<board->M;block_col++){/*for each block in the row*/
				print_block_row(board->blocks[block_row][block_col],marking_board->blocks[block_row][block_col],row , mark); /*print the row*/
				printf(" |");
			}
			printf("\n");
		}
	}
	printf("%s",board->line_seperator);
}

/*this function is copying all the cells' value of one block to another block.
 * @param in_block - the block needed to be copied.
 * @return the block that is a copy of in_board
 *
 */
BLOCK *copy_Block(BLOCK *in_block){ /*return a copy of in_block*/
	BLOCK *res;
	res = (BLOCK*)malloc(sizeof(BLOCK));
	if (res == NULL) {
		print_system_error(1, "couldn't copy board");
	}
	res->M = in_block->M;
	res->N = in_block->N;
	res->values = (int**)malloc(res->M*sizeof(int*));
	if (res->values == NULL) {
		print_system_error(1, "couldn't copy board");
	}
	if (res == NULL) {
		print_system_error(1, "couldn't copy board");
	}
	int i=0;
	int j=0;
	for (i=0;i<in_block->M;i++){/*for each row of block*/
		res->values[i] = (int*)malloc(res->N*sizeof(int));
		if (res->values[i] == NULL) {
			print_system_error(1, "couldn't copy board");
		}
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
	BOARD *res = NULL;
	if(in_board == NULL){
		return NULL;
	}
	res = (BOARD*)malloc(sizeof(BOARD));
	if (res == NULL) {
		print_system_error(1, "couldn't copy board");
	}
	res->M = in_board->M;
	res->N = in_board->N;
	strcpy(res->line_seperator,in_board->line_seperator);
	res->blocks = (BLOCK***)malloc(res->N*sizeof(BLOCK**));
	if (res->blocks == NULL) {
		print_system_error(1, "couldn't copy board");
	}
	int i=0;
	int j=0;
	for (i=0;i<in_board->N;i++){/*for each row of blocks in board*/
		res->blocks[i] = (BLOCK**)malloc(res->M*sizeof(BLOCK*));
		if (res->blocks[i] == NULL) {
			print_system_error(1, "couldn't copy board");
		}
		for(j=0;j<in_board->M;j++){/*for each column of blocks in board*/
			res->blocks[i][j] = copy_Block(in_board->blocks[i][j]);/*copy block*/
		}
	}
	return res;
}
