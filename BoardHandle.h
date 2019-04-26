/*
 * BoardHandle.h - storing all the types_def,constants, and functions regarding the sudoko board.
 *x - col, starting from 0;
 *y - row, starting from 0;
 *
 *get_element_from_board     - getting the element in x,y cell. 0 means no value
 *set_element_to_board 		 - setting a digit in a cell.
 *zero_board				 - initializing a board with zeros
 *is_valid_insertion		 - checking if the digit can be in the cell by the laws of the game
 *print_board                - printing the board
 *copy_board                 - copying a board to another one
 */
#include <stdlib.h>
#include <stdio.h>
#include "strings.h"
#include "Errors.h"
#include "AuxModule.h"

#ifndef BOARDHANDLER_H_
#define BOARDHANDLER_H_

typedef struct bl {/*BLOCK is a M X N matrix of integers*/
	int N, M, **values;
}BLOCK;

typedef struct bo{ /*BOARD is a N X M matrix of BLOCKs*/
	int N, M;
	BLOCK*** blocks;
	char line_seperator[1024];/*the separator row for printing*/
}BOARD;

/*
 * this function initializes two empty boards with N rows and M columns of blocks.
 *
 * @param board1 - address to put the first empty board.
 * @param board2 - address to put the second empty board.
 *
 * @param N - # of rows of blocks.
 * @param M - # of cols of blocks.
 *
 */
void init_boards(BOARD** board1, BOARD** board2, int N, int M);

/*
 * this function releasing all the memory allocated for a board.
 *
 * @param board - the board needed to be deleted.
 */
void delete_board(BOARD* board);

/*
 * this function return a value of a cell in the board
 * @param board - the board.
 * @param x - the cell's col.
 * @param y - the cell's row.
 * @return the element in cell x,y. 0 is empty cell
 *
 */
int get_element_from_board(BOARD *board, int x,int y);

/*
 * this function sets a value to a cell in the board
 * @param board - the board.
 * @param x - the cell's col.
 * @param y - the cell's row.
 * @param z - the desired value. 0 means to empty the cell.
 *
 */
void set_element_to_board(BOARD *board, int x,int y,int z);


/*
 * checks if this column in board contains z
 * @param board - the board of interest.
 * @param column- the column to check
 * @param z     - the value check
 * @return
 * 1 - if the column doesnt have the value z
 * 0 - otherwise
 */
int is_valid_column(BOARD *board,int column,int z);


/*
 * checks if this row in board contains z
 * @param board - the board of interest.
 * @param row   - the column to check
 * @param z     - the value check
 * @return
 * 1 - if the row doesn't have the value z
 * 0 - otherwise
 */
int is_valid_row(BOARD *board,int row,int z);

/*
 * checks if this block contains z
 * @param block - the board of interest.
 * @param z     - the value check
 * @return
 * 1 - if the blcok doesn't have the value z
 * 0 - otherwise
 */
int is_valid_block(BLOCK *block,int z);



int is_valid_insertion_to_empty_cell(BOARD *board,int x,int y,int z);

/*
 * this function validate that an insertion to a cell is legal (uniqueness in row, column and block)
 * @param board - the board.
 * @param x - the cell's col.
 * @param y - the cell's row.
 * @param z - the value needed to be checked if legal.
 * @return 1 if z can be insert to the board in x,y. 0 if not
 *
 */
int is_valid_insertion(BOARD *board,int x,int y,int z);

int is_valid_board(BOARD *board, BOARD *fixed_board, int* isValidBoard, int* isUpdatedBoard);

/*
 * this function prints the board
 * @param board       - the board needed to be printed.
 * @param fixed_board - the fixed board. the function needs it to print "." before fixed values, or " " before not-fixed values.
 *
 */
void print_board(BOARD *board, BOARD *fixed_board, int mark_errors, int mode, int* isValidBoard, int* isUpdatedBoard);

/*this function is copying all the cells' value of one board to another board.
 * @param in_board  - the board needed to be copied.
 * @return - the board that is a copy of in_board
 *
 */
BOARD *copy_board(BOARD *in_board);

#endif /* BOARDHANDLER_H_ */
