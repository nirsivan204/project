/*
 * LP_handler.h
 *
 *  Created on: 12 במרס 2019
 *      Author: nir
 */

#ifndef LP_HANDLER_H_
#define LP_HANDLER_H_
#include "BoardHandle.h"
int map_maker(BOARD *board,int *map, int nXm,int nXm_square);
int row_constraint(BOARD *board, int *map, int row, int digit,int *constraints, int nXm, int nXm_square);
int col_constraint(BOARD *board,int *map, int col, int digit,int *constraints, int nXm, int nXm_square);
int cell_constraint(BOARD *board,int *map, int row,int col,int *constraints, int nXm, int nXm_square);
int block_constraint(BOARD *board,int *map,int block_row, int block_col,int digit,int *constraints,int nXm, int nXm_square);

#endif /* LP_HANDLER_H_ */
