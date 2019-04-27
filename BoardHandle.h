/*
 * BoardHandle.h - storing all the types_def,constants, and functions regarding the Sudoku board.
 *x - col, starting from 0;
 *y - row, starting from 0;
 *
 * This module supports the following functions:
 * init_boards						- initializing two empty boards
 * delete_board						- releasing all the memory allocated for a board
 * get_element_from_board   		- getting the element in x,y cell. 0 means no value
 * set_element_to_board 			- setting a digit in a cell.
 * is_valid_column					- checking if a column in board contains a specific digit
 * is_valid_row						- checking if a row in board contains a specific digit
 * is_valid_block					- checking if a block in board contains a specific digit
 * is_valid_insertion_to_empty_cell	- checking if the digit can be set in an empty cell
 * is_valid_insertion				- checking if the digit can be set in a cell
 * is_valid_board					- checking if there are no erroneous cells in board
 * print_board                		- printing the board
 * copy_board                		- copying a board to another one
 */
#include <stdlib.h>
#include <stdio.h>
#include "strings.h"
#include "Errors.h"

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
 * this function checks if this block contains z
 * @param block - the board of interest.
 * @param z     - the value check
 * @return
 * 1 - if the blcok doesn't have the value z
 * 0 - otherwise
 */
int is_valid_block(BLOCK *block,int z);

/*
 * this function validates that an insertion to an empty cell is legal (uniqueness in row, column and block)
 * @param board - the board.
 * @param x - the cell's col.
 * @param y - the cell's row.
 * @param z - the value needed to be checked if legal.
 * @return 1 if z can be inserted to the board in cell<x,y>. 0 if not.
 *
 */
int is_valid_insertion_to_empty_cell(BOARD *board,int x,int y,int z);

/*
 * this function validates that an insertion to a cell is legal (uniqueness in row, column and block)
 * @param board - the board.
 * @param x - the cell's col.
 * @param y - the cell's row.
 * @param z - the value needed to be checked if legal.
 * @return 1 if z can be inserted to the board in cell<x,y>. 0 if not.
 *
 */
int is_valid_insertion(BOARD *board,int x,int y,int z);

/*
 * this function makes sure the marking-board is updated (every cell in the game-board has the relevant mark in the marking-board) and checks if the board contains erroneous cells.
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 *  @return 1 if the board doesn't contain erroneous cells, 0 otherwise.
 */
int is_valid_board(BOARD *board, BOARD *marking_board, int* isValidBoard, int* isUpdatedBoard);

/*
 * this function prints the board after making sure the marking-board is updated (every cell in the game-board has the relevant mark in the marking-board).
 *  @param board			- a pointer to the current puzzle.
 *  @param marking_board	- a pointer to a table that contains values which represent each of the puzzle cells' status: normal, fixed or erroneous.
 *  						  The function needs it to print "." before fixed values, or " " before not-fixed values.
 *  @param markErrors		- a number that determines whether errors in the puzzle are displayed.
 *  @param mode				- a number that represents one of the 3 playable modes.
 *  @param isValidBoard		- a pointer to a number that determines whether there are errors in the puzzle.
 *  @param isUpdatedBoard	- a pointer to a number that determines whether a necessary search for errors was made.
 */
void print_board(BOARD *board, BOARD *marking_board, int mark_errors, int mode, int* isValidBoard, int* isUpdatedBoard);

/* this function is copying all the cells' value of one board to another board.
 * @param in_board  - the board needed to be copied.
 *
 * @return - the board that is a copy of in_board
 */
BOARD *copy_board(BOARD *in_board);

#endif /* BOARDHANDLER_H_ */
