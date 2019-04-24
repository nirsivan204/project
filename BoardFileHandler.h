/*
 * BoardFileHandler.h
 *
 *  Created on: 13 αταψ 2019
 *      Author: nir
 */

#include "BoardHandle.h"
#include "Errors.h"

#ifndef BOARDFILEHANDLER_H_
#define BOARDFILEHANDLER_H_

/*
 * This function is opening a file specified in path, parsing it
 * and putting it in the pointer of board,and fix_board;
 *
 * @param path - the path to the file.
 * @param board - the address to put the board in.
 * @param fix_board - the address to put the fixed board in.
 * @param command_name - edit or solve. if solve, the fix board will be filled, else it will be empty board.
 *
 * @return True if success, False else.
 *
 */

int load_board(char *path,BOARD **board, BOARD **fix_board, int command_name);

/*
 * This function is saving the board (and the fix_board) to a file specified in path.
 *
 * @param path - the path to the file.
 * @param board - the board to save.
 * @param fix_board - the fix_board correspond to the board it saves.
 * @param mode - if mode == edit, it will save every element!=0 as fixed, else, it will save as fixed only the fixed values.
 *
 * @return True if success, False else.
 *
 */
int save_board(char *path,BOARD *board,BOARD *fix_board,int mode);


#endif /* BOARDFILEHANDLER_H_ */


