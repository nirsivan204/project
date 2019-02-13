/*
 * BoardFileHandler.h
 *
 *  Created on: 13 αταψ 2019
 *      Author: nir
 */

#include "BoardHandle.h"

#ifndef BOARDFILEHANDLER_H_
#define BOARDFILEHANDLER_H_

void load_board(char *path,BOARD *board, BOARD *fix_board);
void save_board(char *path,BOARD *board,BOARD *fix_board,int mode);


#endif /* BOARDFILEHANDLER_H_ */


