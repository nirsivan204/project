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

int load_board(char *path,BOARD *board, BOARD *fix_board, int command_name);
int save_board(char *path,BOARD *board,BOARD *fix_board,int mode);


#endif /* BOARDFILEHANDLER_H_ */


